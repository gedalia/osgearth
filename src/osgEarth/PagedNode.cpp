/* -*-c++-*- */
/* osgEarth - Geospatial SDK for OpenSceneGraph
 * Copyright 2020 Pelican Mapping
 * http://osgearth.org
 *
 * osgEarth is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include <osgEarth/PagedNode>
#include <osgEarth/Utils>
#include <osgEarth/GLUtils>

#include <osgDB/Registry>
#include <osgDB/FileNameUtils>

#define LC "[PagedNode] "

using namespace osgEarth;
using namespace osgEarth::Util;

namespace
{    
    struct PagedNodePseudoLoader : public osgDB::ReaderWriter
    {
        PagedNodePseudoLoader()
        {
            supportsExtension( "osgearth_pseudo_pagednode", "" );
        }

        const char* className() const
        { // override
            return "PagedNodePseudoLoader";
        }

        ReadResult readNode(const std::string& uri, const Options* options) const
        {
            if ( !acceptsExtension( osgDB::getLowerCaseFileExtension(uri) ) )
                return ReadResult::FILE_NOT_HANDLED;

            osg::ref_ptr<PagedNode> node;
            if (!ObjectStorage::get(options, node))
            {
                OE_WARN << "Internal error - no PagedNode object in ObjectStorage\n";
                return ReadResult::ERROR_IN_READING_FILE;
            }

            return node->loadChild();
        }
    };

    REGISTER_OSGPLUGIN(osgearth_pseudo_pagednode, PagedNodePseudoLoader);
}

PagedNode::PagedNode() :
    _rangeFactor(6.0f),
    _additive(false)
{
    _plod = new osg::PagedLOD;
    addChild(_plod);

    _attachPoint = new osg::Group;

    _plod->addChild( _attachPoint );     
}

void PagedNode::setRangeMode(osg::LOD::RangeMode mode)
{
    _plod->setRangeMode(mode);
}

void PagedNode::setNode(osg::Node* node)
{
    if (node)
        _attachPoint->addChild(node);
}

void PagedNode::setupPaging()
{
    osg::BoundingSphere bs = getChildBound();

    _plod->setCenter( bs.center() ); 
    _plod->setRadius( bs.radius() );

    if ( hasChild() )
    {    
        // Now setup a filename on the PagedLOD that will load all of the children of this node.
        _plod->setFileName(1, ".osgearth_pseudo_pagednode");
      
        // assemble data to pass to the pseudoloader
        osgDB::Options* options = new osgDB::Options();
        ObjectStorage::set(options, this);
        _plod->setDatabaseOptions( options );

        // Setup the min and max ranges.
        float minRange;
        if ( _range.isSet() )
        {
            minRange = _range.get();
        }
        else
        {
            if (_plod->getRangeMode() == _plod->DISTANCE_FROM_EYE_POINT)
            {                
                minRange = (float)(bs.radius() * _rangeFactor);
            }
            else
            {
                minRange = 256;
            }
        }

        if (!_additive)
        {
            // Replace mode, the parent is replaced by its children.
            if (_plod->getRangeMode() == _plod->DISTANCE_FROM_EYE_POINT)
            {
                _plod->setRange( 0, minRange, FLT_MAX );
                _plod->setRange( 1, 0, minRange );
            }
            else
            {
                _plod->setRange(0, 0, minRange);
                _plod->setRange(1, minRange, FLT_MAX);
            }
        }
        else
        {
            // Additive, the parent remains and new data is added
            if (_plod->getRangeMode() == _plod->DISTANCE_FROM_EYE_POINT)
            {
                _plod->setRange( 0, 0, FLT_MAX );
                _plod->setRange( 1, 0, minRange );
            }
            else
            {
                _plod->setRange(0, 0, FLT_MAX);
                _plod->setRange(1, minRange, FLT_MAX);
            }
        }
    }
    else
    {
        // no children, so max out the visibility range.
        _plod->setRange( 0, 0, FLT_MAX );
    }   
}

osg::BoundingSphere PagedNode::getChildBound() const
{
    return osg::BoundingSphere();
}

bool PagedNode::hasChild() const
{
    return true;
}






void
PagedNode2::merge()
{
    const osg::ref_ptr<osg::Node>& node = _result.get();
    if (node.valid())
    {
        addChild(node);
    }
}

void
PagedNode2::traverse(osg::NodeVisitor& nv)
{
}





PagingManager::PagingManager() :
    _mutex(OE_MUTEX_NAME)
{
    //nop
}

void
PagingManager::merge(PagedNode2* host)
{
    OE_SOFT_ASSERT_AND_RETURN(host != nullptr, __func__, );

    ScopedMutexLock lock(_mutex);

    if (_ico.isSet() && _ico.get().valid())
    {
        ScopedMutexLock lock(_mutex);

        GLObjectsCompiler glcompiler;

        _compileQueue.emplace(std::make_shared<PagingOperation>());
        PagingOperationPtr& op = _compileQueue.back();

        op->_host = host;
        op->_compileJob = glcompiler.compileAsync(
            host->_result.get(),
            this,
            nullptr);
    }
    else
    {
        _mergeQueue.emplace(std::make_shared<PagingOperation>());
        PagingOperationPtr& op = _compileQueue.back();
        op->_host = host;
    }
}

void
PagingManager::traverse(osg::NodeVisitor& nv)
{
    if (!_ico.isSet() && nv.getVisitorType() == nv.CULL_VISITOR)
    {
        // detect presence of an ICO
        osg::ref_ptr<osgUtil::IncrementalCompileOperation> temp;
        ObjectStorage::get(&nv, temp);
        _ico = temp.get();
    }

    else if (nv.getVisitorType() == nv.UPDATE_VISITOR)
    {
        //todo: check the queues...
        ScopedMutexLock lock(_mutex);

        while (!_compileQueue.empty())
        {
            PagingOperationPtr& op = _compileQueue.front();
            if (op->_compileJob.isAvailable())
            {
                _mergeQueue.emplace(std::move(op));
                _compileQueue.pop();
            }
            else if (op->_compileJob.isAbandoned())
            {
                _compileQueue.pop(); // discard
            }
            else break;
        }

        while (!_mergeQueue.empty()) // TODO: meter it
        {
            PagingOperationPtr& op = _mergeQueue.front();
            osg::ref_ptr<PagedNode2> host;
            if (op->_host.lock(host))
            {
                host->merge();
            }
            _mergeQueue.pop();
        }
    }

    osg::Group::traverse(nv);
}