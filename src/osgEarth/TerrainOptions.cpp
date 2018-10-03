/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
 * Copyright 2016 Pelican Mapping
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

#include <osgEarth/TerrainOptions>

using namespace osgEarth;

//----------------------------------------------------------------------------

TerrainOptions::TerrainOptions( const ConfigOptions& options ) :
DriverConfigOptions( options ),
_tileSize( 17 ),
_verticalScale( 1.0f ),
_verticalOffset( 0.0f ),
_minTileRangeFactor( 7.0 ),
_combineLayers( true ),
_maxLOD( 23 ),
_minLOD( 0 ),
_firstLOD( 0 ),
_enableLighting( false ),
_attenuationDistance( 10000.0f ),
_lodTransitionTimeSeconds( 0.5f ),
_enableMipmapping( true ),
_clusterCulling( true ),
_enableBlending( true ),
_compressNormalMaps( false ),
_mercatorFastPath( false ),
_minFilter( osg::Texture::LINEAR_MIPMAP_LINEAR ),
_magFilter( osg::Texture::LINEAR),
_minNormalMapLOD( 0u ),
_gpuTessellation( false ),
_debug( false ),
_binNumber( 0 ),
_castShadows(true),
_rangeMode(osg::LOD::DISTANCE_FROM_EYE_POINT),
_tilePixelSize(256)
{
    fromConfig( _conf );
}

Config
TerrainOptions::getConfig() const
{
    Config conf = DriverConfigOptions::getConfig();
    conf.key() = "terrain";
    
    conf.set( "tile_size", _tileSize );
    conf.set( "vertical_scale", _verticalScale );
    conf.set( "vertical_offset", _verticalOffset );
    conf.set( "min_tile_range_factor", _minTileRangeFactor );
    conf.set( "range_factor", _minTileRangeFactor );  
    conf.set( "max_lod", _maxLOD );
    conf.set( "min_lod", _minLOD );
    conf.set( "first_lod", _firstLOD );
    conf.set( "lighting", _enableLighting );
    conf.set( "attenuation_distance", _attenuationDistance );
    conf.set( "lod_transition_time", _lodTransitionTimeSeconds );
    conf.set( "mipmapping", _enableMipmapping );
    conf.set( "cluster_culling", _clusterCulling );
    conf.set( "blending", _enableBlending );
    conf.set( "compress_normal_maps", _compressNormalMaps);
    conf.set( "mercator_fast_path", _mercatorFastPath );
    conf.set( "min_normal_map_lod", _minNormalMapLOD );
    conf.set( "gpu_tessellation", _gpuTessellation );
    conf.set( "debug", _debug );
    conf.set( "bin_number", _binNumber );
    conf.set( "min_expiry_time", _minExpiryTime);
    conf.set( "min_expiry_frames", _minExpiryFrames);
    conf.set("cast_shadows", _castShadows);
    conf.set("tile_pixel_size", _tilePixelSize);
    conf.set("range_mode", "PIXEL_SIZE_ON_SCREEN", _rangeMode, osg::LOD::PIXEL_SIZE_ON_SCREEN);
    conf.set("range_mode", "DISTANCE_FROM_EYE_POINT", _rangeMode, osg::LOD::DISTANCE_FROM_EYE_POINT);

    //Save the filter settings
	conf.set("mag_filter","LINEAR",                _magFilter,osg::Texture::LINEAR);
    conf.set("mag_filter","LINEAR_MIPMAP_LINEAR",  _magFilter,osg::Texture::LINEAR_MIPMAP_LINEAR);
    conf.set("mag_filter","LINEAR_MIPMAP_NEAREST", _magFilter,osg::Texture::LINEAR_MIPMAP_NEAREST);
    conf.set("mag_filter","NEAREST",               _magFilter,osg::Texture::NEAREST);
    conf.set("mag_filter","NEAREST_MIPMAP_LINEAR", _magFilter,osg::Texture::NEAREST_MIPMAP_LINEAR);
    conf.set("mag_filter","NEAREST_MIPMAP_NEAREST",_magFilter,osg::Texture::NEAREST_MIPMAP_NEAREST);
    conf.set("min_filter","LINEAR",                _minFilter,osg::Texture::LINEAR);
    conf.set("min_filter","LINEAR_MIPMAP_LINEAR",  _minFilter,osg::Texture::LINEAR_MIPMAP_LINEAR);
    conf.set("min_filter","LINEAR_MIPMAP_NEAREST", _minFilter,osg::Texture::LINEAR_MIPMAP_NEAREST);
    conf.set("min_filter","NEAREST",               _minFilter,osg::Texture::NEAREST);
    conf.set("min_filter","NEAREST_MIPMAP_LINEAR", _minFilter,osg::Texture::NEAREST_MIPMAP_LINEAR);
    conf.set("min_filter","NEAREST_MIPMAP_NEAREST",_minFilter,osg::Texture::NEAREST_MIPMAP_NEAREST);

    return conf;
}

void
TerrainOptions::fromConfig( const Config& conf )
{
    conf.get( "tile_size", _tileSize );
    conf.get( "vertical_scale", _verticalScale );
    conf.get( "vertical_offset", _verticalOffset );
    conf.get( "min_tile_range_factor", _minTileRangeFactor );   
    conf.get( "range_factor", _minTileRangeFactor );   
    conf.get( "max_lod", _maxLOD ); conf.get( "max_level", _maxLOD );
    conf.get( "min_lod", _minLOD ); conf.get( "min_level", _minLOD );
    conf.get( "first_lod", _firstLOD ); conf.get( "first_level", _firstLOD );
    conf.get( "lighting", _enableLighting );
    conf.get( "attenuation_distance", _attenuationDistance );
    conf.get( "lod_transition_time", _lodTransitionTimeSeconds );
    conf.get( "mipmapping", _enableMipmapping );
    conf.get( "cluster_culling", _clusterCulling );
    conf.get( "blending", _enableBlending );
    conf.get( "compress_normal_maps", _compressNormalMaps);
    conf.get( "mercator_fast_path", _mercatorFastPath );
    conf.get( "min_normal_map_lod", _minNormalMapLOD );
    conf.get( "gpu_tessellation", _gpuTessellation );
    conf.get( "debug", _debug );
    conf.get( "bin_number", _binNumber );
    conf.get( "min_expiry_time", _minExpiryTime);
    conf.get( "min_expiry_frames", _minExpiryFrames);
    conf.get("cast_shadows", _castShadows);
    conf.get("tile_pixel_size", _tilePixelSize);
    conf.get("range_mode", "PIXEL_SIZE_ON_SCREEN", _rangeMode, osg::LOD::PIXEL_SIZE_ON_SCREEN);
    conf.get("range_mode", "DISTANCE_FROM_EYE_POINT", _rangeMode, osg::LOD::DISTANCE_FROM_EYE_POINT);

    //Load the filter settings
	conf.get("mag_filter","LINEAR",                _magFilter,osg::Texture::LINEAR);
    conf.get("mag_filter","LINEAR_MIPMAP_LINEAR",  _magFilter,osg::Texture::LINEAR_MIPMAP_LINEAR);
    conf.get("mag_filter","LINEAR_MIPMAP_NEAREST", _magFilter,osg::Texture::LINEAR_MIPMAP_NEAREST);
    conf.get("mag_filter","NEAREST",               _magFilter,osg::Texture::NEAREST);
    conf.get("mag_filter","NEAREST_MIPMAP_LINEAR", _magFilter,osg::Texture::NEAREST_MIPMAP_LINEAR);
    conf.get("mag_filter","NEAREST_MIPMAP_NEAREST",_magFilter,osg::Texture::NEAREST_MIPMAP_NEAREST);
    conf.get("min_filter","LINEAR",                _minFilter,osg::Texture::LINEAR);
    conf.get("min_filter","LINEAR_MIPMAP_LINEAR",  _minFilter,osg::Texture::LINEAR_MIPMAP_LINEAR);
    conf.get("min_filter","LINEAR_MIPMAP_NEAREST", _minFilter,osg::Texture::LINEAR_MIPMAP_NEAREST);
    conf.get("min_filter","NEAREST",               _minFilter,osg::Texture::NEAREST);
    conf.get("min_filter","NEAREST_MIPMAP_LINEAR", _minFilter,osg::Texture::NEAREST_MIPMAP_LINEAR);
    conf.get("min_filter","NEAREST_MIPMAP_NEAREST",_minFilter,osg::Texture::NEAREST_MIPMAP_NEAREST);
}
