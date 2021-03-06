#!/bin/bash

##################################################################################
# Authors: Shireen Elhabian
# Date:    Spring 2018
# Company: SCI Institute, Univerity of Utah
# Project: CIBC
# Purpose: Run shapeworks prep pipeline given a set of meshes 
# Notes:
##################################################################################

### [1] Preview : Does mesh decimation and cleaning to speed up the computation for fids
doPreview=1
decimation_decimal=0.99
decimation_percentage=99

### [2] Smoothing : Performs Mesh Smoothing
doSmooth=1
smoothing_iterations=1
relaxation_factor=0.5

### [3] Alignment : Performs Mesh Alignment
doAlign=1
registration_mode=similarity
reference_meshID=ref

### [4] Size and Origin : Computes the biggest bounding box size and origin
doSizeOrigin=1
spacing=0.5
narrow_band=10

### [5] Mesh to DT : Computes parameterfile and then the DT from the meshes
doMeshesToVolumes=1
ball_radius_factor=1

### [6] Fix DT Leakage : Fixes leakage in the distance transforms for the fids
doFixFidsDT=1

### [7] Topology Preserving Smoothing : Merforms smoothing on the DT tranforms
doTPSmoothDT=0
TPsmoothing_iterations=1

scriptHome=../ # this is the Scripts folder in ShapeworksPrep directory

parentDir='' # this is where all the preprocessed data will lie
rawDataDir='' # this is the path to the data (raw Images and Segmentations)
mesh_extension='vtk'

while [[ $# > 1 ]]
do
  key="$1"
  case $key in
  
      -p|--parent_dir)
      parentDir="$2"
      shift
      ;;
      
      -i|--raw_data_dir) # prefix of mri images
      rawDataDir="$2"
      shift
      ;;
      
      -g|--mesh_extension)
      mesh_extension="$2"
      shift
      ;;
      
      -r|--reference_meshID)
      reference_meshID="$2"
      shift
      ;;

      -r|--do_preview)
      doPreview="$2"
      shift
      ;;

      -r|--do_smooth)
      doSmooth="$2"
      shift
      ;;
      
      -s|--scriptHome)
      scriptHome="$2"
      shift
      ;;

      --default)
      DEFAULT=YES
      shift
      ;;
      *)
        # unknown option
      ;;
  esac
  shift
done

source ${scriptHome}/setup.txt # works for server as well
source ${scriptHome}/Utils/Utils.sh # common utility functions
scriptDir=${scriptHome}/ToolLevelScripts/

parentDir=${parentDir}/
rawDataDir=${rawDataDir}/
buffer_distance=2

if [ $doPreview -eq 1 ]
then

    ${scriptDir}/MeshesPreviewQualityControl.sh  --data_dir ${rawDataDir} \
                                --out_dir ${parentDir}previewed/ \
                                --mesh_extension $mesh_extension \
                                --decimation_decimal $decimation_decimal \
                                --decimation_percentage $decimation_percentage \
                                --use_preview 1 --scriptHome $scriptHome
                                
                                
fi
if [ $doPreview -eq 0 ]
then
  mkdir -p ${parentDir}previewed
  for filename in $(find $rawDataDir -name "*.${mesh_extension}" | sort -t '\0' ) ;
  do
    prefix=$( GetFilePrefix ${filename} )
    outnm=${parentDir}previewed/${prefix}.preview${decimation_percentage}.${mesh_extension}
    cp $filename ${parentDir}previewed/
    mv ${parentDir}previewed/${prefix}.${mesh_extension} $outnm
  done
fi


if [ $doSmooth -eq 1 ]
then

    ${scriptDir}/SmoothMeshes.sh  --data_dir ${parentDir}previewed/  \
                      --out_dir ${parentDir}smoothed/ \
                      --mesh_extension $mesh_extension \
                      --smoothing_iterations $smoothing_iterations --relaxation_factor $relaxation_factor \
                      --mesh_suffix preview${decimation_percentage} --scriptHome $scriptHome
                      
fi
if [ $doSmooth -eq 0 ]
then
  mkdir -p ${parentDir}smoothed
  for filename in $(find ${parentDir}previewed/ -name "*preview${decimation_percentage}.${mesh_extension}" | sort -t '\0' ) ;
  do
    prefix=$( GetFilePrefix ${filename} )
    outnm=${parentDir}smoothed/${prefix}.smooth${smoothing_iterations}.${mesh_extension}
    cp $filename ${parentDir}smoothed/
    mv ${parentDir}smoothed/${prefix}.${mesh_extension} $outnm
  done
fi


if [ $doAlign -eq 1 ]
then
    reference_mesh=${parentDir}smoothed/${reference_meshID}.preview${decimation_percentage}.smooth${smoothing_iterations}.ply 

    ${scriptDir}/AlignMeshes.sh  --data_dir ${parentDir}smoothed/ \
                      --out_dir ${parentDir}aligned/ \
                      --mesh_suffix preview${decimation_percentage}.smooth${smoothing_iterations} \
                      --mesh_extension $mesh_extension \
                      --reference_mesh $reference_mesh \
                      --registration_mode $registration_mode --scriptHome $scriptHome

fi

# compute the origin and size of the distance transform volume where all meshes will fit into
if [ $doSizeOrigin -eq 1 ]
then
    
    ${scriptDir}/RasterizationVolumeOriginAndSize.sh --data_dir ${parentDir}aligned/ \
                                          --out_dir ${parentDir}origin_size/ \
                                          --mesh_suffix preview${decimation_percentage}.smooth${smoothing_iterations}.similarity_icp \
                                          --spacing $spacing --narrow_band $narrow_band --scriptHome $scriptHome
                                         
fi

# convert meshes to distance transforms
if [ $doMeshesToVolumes -eq 1 ]
then
    origin_filename=${parentDir}origin_size/origin_sp${spacing}_nb${narrow_band}.txt
    size_filename=${parentDir}origin_size/size_sp${spacing}_nb${narrow_band}.txt

    ${scriptDir}/MeshesToVolumesWithOriginAndSize.sh  --data_dir ${parentDir}aligned/ \
                                --out_dir ${parentDir}fidsDT/ \
                                --mesh_extension $mesh_extension \
                                --origin_filename $origin_filename\
                                --size_filename $size_filename\
                                --mesh_suffix preview${decimation_percentage}.smooth${smoothing_iterations}.similarity_icp \
                                --spacing $spacing --narrow_band $narrow_band\
                                --ball_radius_factor $ball_radius_factor \
                                --num_meshes_in_parallel 5 --num_threads 20 --scriptHome $scriptHome
fi



if [ $doFixFidsDT -eq 1 ]
then
    ${scriptDir}/RemoveFidsDTLeakage.sh --data_dir ${parentDir}fidsDT/ \
                             --out_dir ${parentDir}fidsDT/ \
                             --mesh_extension $mesh_extension \
                             --mesh_suffix preview${decimation_percentage}.smooth${smoothing_iterations}.similarity_icp \
                             --dt_suffix DT_r${ball_radius_factor}_sp${spacing}_nb${narrow_band} \
                             --fids_dt_suffix SignedDistMap_r${ball_radius_factor}_sp${spacing}_nb${narrow_band} \
                             --scriptHome $scriptHome
                                          
fi

if [ $doTPSmoothDT -eq 1 ]
then
    ${scriptDir}/TopologyPreservingDTSmoothing.sh  --data_dir ${parentDir}fidsDT/ \
                                        --out_dir ${parentDir}groom/ \
                                        --dt_suffix preview${decimation_percentage}.smooth${smoothing_iterations}.similarity_icp.SignedDistMap_r${ball_radius_factor}_sp${spacing}_nb${narrow_band}.fixed \
                                        --smoothing_iterations $TPsmoothing_iterations --scriptHome $scriptHome
                                
    mkdir -p ${parentDir}DistanceTransforms/
    mv ${parentDir}groom/*.tpSmoothDT.nrrd ${parentDir}DistanceTransforms/                             
fi
