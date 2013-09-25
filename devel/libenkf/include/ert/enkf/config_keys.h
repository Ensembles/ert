/*
   Copyright (C) 2011  Statoil ASA, Norway. 
    
   The file 'config_keys.h' is part of ERT - Ensemble based Reservoir Tool. 
    
   ERT is free software: you can redistribute it and/or modify 
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation, either version 3 of the License, or 
   (at your option) any later version. 
    
   ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
   FITNESS FOR A PARTICULAR PURPOSE.   
    
   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
   for more details. 
 */


#ifndef  __CONFIG_KEYS_H__
#define  __CONFIG_KEYS_H__
#ifdef   __cplusplus
extern "C" {
#endif

/* These keys are used as options in KEY:VALUE statements */

/* Config options related to the ECLIPSE model ecl_config.c*/
#define  SCHEDULE_FILE_KEY                 "SCHEDULE_FILE"
#define  ECLBASE_KEY                       "ECLBASE"
#define  DATA_FILE_KEY                     "DATA_FILE"
#define  STATIC_KW_KEY                     "ADD_STATIC_KW"
#define  ADD_FIXED_LENGTH_SCHEDULE_KW_KEY  "ADD_FIXED_LENGTH_SCHEDULE_KW"
#define  REFCASE_KEY                       "REFCASE"
#define  REFCASE_LIST_KEY                  "REFCASE_LIST"
#define  PLOT_REFCASE_LIST_KEY             "PLOT_REFCASE_LIST"
#define  GRID_KEY                          "GRID"
#define  INIT_SECTION_KEY                  "INIT_SECTION"
#define  END_DATE_KEY                      "END_DATE"
#define  SCHEDULE_PREDICTION_FILE_KEY      "SCHEDULE_PREDICTION_FILE"
/* Config options in model_config.c*/
#define  FORWARD_MODEL_KEY                 "FORWARD_MODEL"
#define  ENKF_SCHED_FILE_KEY               "ENKF_SCHED_FILE"
#define  RUNPATH_KEY                       "RUNPATH"
#define  RERUN_PATH_KEY                    "RERUN_PATH" /*Not implemented in fprintf_config*/
#define  HISTORY_SOURCE_KEY                "HISTORY_SOURCE"
#define  CASE_TABLE_KEY                    "CASE_TABLE"
#define  ENSPATH_KEY                       "ENSPATH" 
#define  JOBNAME_KEY                       "JOBNAME"
#define  RFTPATH_KEY                       "RFTPATH"
#define  DBASE_TYPE_KEY                    "DBASE_TYPE" 
#define  MAX_RESAMPLE_KEY                  "MAX_RESAMPLE"
#define  SELECT_CASE_KEY                   "SELECT_CASE"
#define  NUM_REALIZATIONS_KEY              "NUM_REALIZATIONS"      
/* Config options in ens_obs.c*/
#define  OBS_CONFIG_KEY                    "OBS_CONFIG"
/* Config options in analysis_config.c*/
#define  ENKF_MERGE_OBSERVATIONS_KEY       "ENKF_MERGE_OBSERVATIONS"
#define  UPDATE_RESULTS_KEY                "UPDATE_RESULTS"
#define  STD_CUTOFF_KEY                    "STD_CUTOFF"
#define  ENKF_ALPHA_KEY                    "ENKF_ALPHA"
#define  SINGLE_NODE_UPDATE_KEY            "SINGLE_NODE_UPDATE"
#define  ENKF_RERUN_KEY                    "ENKF_RERUN"
#define  RERUN_START_KEY                   "RERUN_START"
#define  UPDATE_LOG_PATH_KEY               "UPDATE_LOG_PATH"
#define  MIN_REALIZATIONS_KEY              "MIN_REALIZATIONS" 
/*
#define  ENKF_CROSS_VALIDATION_KEY         "ENKF_CROSS_VALIDATION"
#define  ENKF_LOCAL_CV_KEY                 "ENKF_LOCAL_CV"     
#define  ENKF_PEN_PRESS_KEY                "ENKF_PEN_PRESS"
#define  ENKF_SCALING_KEY                  "ENKF_SCALING"     
#define  ENKF_KERNEL_REG_KEY               "ENKF_KERNEL_REGRESSION"     
#define  ENKF_KERNEL_FUNC_KEY              "ENKF_KERNEL_FUNCTION"     
#define  ENKF_KERNEL_PARAM_KEY             "ENKF_KERNEL_PARAM"     
#define  ENKF_CV_FOLDS_KEY                 "ENKF_CV_FOLDS"     
#define  ENKF_FORCE_NCOMP_KEY              "ENKF_FORCE_NCOMP"
#define  ENKF_NCOMP_KEY                    "ENKF_NCOMP"     */

#define  ANALYSIS_COPY_KEY                 "ANALYSIS_COPY" /*Not implemented in fprintf_config*/
#define  ANALYSIS_LOAD_KEY                 "ANALYSIS_LOAD" /*Not implemented in fprintf_config*/
#define  ANALYSIS_SET_VAR_KEY              "ANALYSIS_SET_VAR" /*Not implemented in fprintf_config*/
#define  ANALYSIS_SELECT_KEY               "ANALYSIS_SELECT" /*Not implemented in fprintf_config*/

/* Config options in analysis_config.c*/
#define  GEN_DATA_KEY                      "GEN_DATA"
#define  GEN_KW_KEY                        "GEN_KW"
#define  GEN_KW_TAG_FORMAT_KEY             "GEN_KW_TAG_FORMAT"
#define  GEN_PARAM_KEY                     "GEN_PARAM" 
#define  SUMMARY_KEY                       "SUMMARY"  
#define  SURFACE_KEY                       "SURFACE"
#define  CONTAINER_KEY                     "CONTAINER"
#define  FIELD_KEY                         "FIELD"
#define  INPUT_FORMAT_KEY                  "INPUT_FORMAT"
#define  OUTPUT_FORMAT_KEY                 "OUTPUT_FORMAT"
#define  INIT_FILES_KEY                    "INIT_FILES"
#define  TEMPLATE_KEY                      "TEMPLATE"
#define  KEY_KEY                           "KEY" 
#define  RESULT_FILE_KEY                   "RESULT_FILE"
#define  MIN_STD_KEY                       "MIN_STD"
#define  FORWARD_INIT_KEY                  "FORWARD_INIT"
#define  BASE_SURFACE_KEY                  "BASE_SURFACE"
#define  MIN_KEY                           "MIN"
#define  MAX_KEY                           "MAX"
#define  OUTPUT_FILE_KEY                   "OUTPUT_FILE"
#define  DYNAMIC_KEY                       "DYNAMIC" 
#define  INIT_TRANSFORM_KEY                "INIT_TRANSFORM"
#define  OUTPUT_TRANSFORM_KEY              "OUTPUT_TRANSFORM"
#define  PARAMETER_KEY                     "PARAMETER"
#define  GENERAL_KEY                       "GENERAL"
#define  INPUT_TRANSFORM_KEY               "INPUT_TRANSFORM"
#define  ECL_FILE_KEY                      "ECL_FILE"
/* Config options in local_config.c*/
#define  LOCAL_CONFIG_KEY                  "LOCAL_CONFIG"
/* Config options printed in enkf_main_fprintf_config*/
#define  PRE_CLEAR_RUNPATH_KEY             "PRE_CLEAR_RUNPATH"
#define  KEEP_RUNPATH_KEY                  "KEEP_RUNPATH"  
#define  DELETE_RUNPATH_KEY                "DELETE_RUNPATH"  
#define  LOG_FILE_KEY                      "LOG_FILE"
#define  LOG_LEVEL_KEY                     "LOG_LEVEL"
#define  RFT_CONFIG_KEY                    "RFT_CONFIG"
#define  DATA_KW_KEY                       "DATA_KW"  /*Implementation for printing in enkf_main_data_kw_fprintf_config present but not in use.*/
#define  INCLUDE_KEY                       "INCLUDE"
#define  DEFINE_KEY                        "DEFINE" 
/*Config options in ert_templates.c*/
#define  RUN_TEMPLATE_KEY                  "RUN_TEMPLATE"
/*Config options in site_config.c*/
#define  INSTALL_JOB_KEY                   "INSTALL_JOB"
#define  SETENV_KEY                        "SETENV"
#define  QUEUE_SYSTEM_KEY                  "QUEUE_SYSTEM"
#define  UMASK_KEY                         "UMASK"   
#define  MAX_SUBMIT_KEY                    "MAX_SUBMIT" 
#define  LICENSE_PATH_KEY                  "LICENSE_PATH"
#define  JOB_SCRIPT_KEY                    "JOB_SCRIPT"
#define  MAX_RUNNING_LOCAL_KEY             "MAX_RUNNING_LOCAL"
#define  MAX_RUNNING_LSF_KEY               "MAX_RUNNING_LSF"
#define  MAX_RUNNING_RSH_KEY               "MAX_RUNNING_RSH"
#define  LSF_QUEUE_KEY                     "LSF_QUEUE"
#define  LSF_RESOURCES_KEY                 "LSF_RESOURCES"
#define  RSH_COMMAND_KEY                   "RSH_COMMAND"
#define  RSH_HOST_KEY                      "RSH_HOST"
#define  UPDATE_PATH_KEY                   "UPDATE_PATH" /*Not printed*/
#define  LSF_SERVER_KEY                    "LSF_SERVER" /*Not printed*/
#define  QUEUE_OPTION_KEY                  "QUEUE_OPTION" /*Not printed*/
/* Report options are currently not printed*/
#define  REPORT_SEARCH_PATH_KEY            "REPORT_SEARCH_PATH"/*Not printed*/
#define  REPORT_CONTEXT_KEY                "REPORT_CONTEXT"/*Not printed*/
#define  REPORT_LARGE_KEY                  "REPORT_LARGE"   /*Not printed*/
#define  REPORT_LIST_KEY                   "REPORT_LIST"/*Not printed*/
#define  REPORT_PATH_KEY                   "REPORT_PATH"/*Not printed*/
#define  REPORT_WELL_LIST_KEY              "REPORT_WELL_LIST"/*Not printed*/
#define  REPORT_GROUP_LIST_KEY             "REPORT_GROUP_LIST"/*Not printed*/
#define  REPORT_TIMEOUT_KEY                "REPORT_TIMEOUT"/*Not printed*/
/*Config options in rng_config.c*/
#define  LOAD_SEED_KEY                     "LOAD_SEED"  
#define  STORE_SEED_KEY                    "STORE_SEED"
/*Config options in analysis_iter_config.c*/
#define  ITER_CASE_KEY                     "ITER_CASE" 
#define  ITER_COUNT_KEY                    "ITER_COUNT"
#define  ITER_RUNPATH_KEY                  "ITER_RUNPATH"
/* QC module options are currently not printed*/
#define  RUNPATH_FILE_KEY                  "RUNPATH_FILE"  
#define  QC_PATH_KEY                       "QC_PATH"  
#define  QC_WORKFLOW_KEY                   "QC_WORKFLOW"
/*Plot options are currently not plotted.*/
#define  IMAGE_TYPE_KEY                    "IMAGE_TYPE"
#define  IMAGE_VIEWER_KEY                  "IMAGE_VIEWER"      
#define  PLOT_DRIVER_KEY                   "PLOT_DRIVER"
#define  PLOT_ERRORBAR_MAX_KEY             "PLOT_ERRORBAR_MAX" 
#define  PLOT_ERRORBAR_KEY                 "PLOT_ERRORBAR"
#define  PLOT_HEIGHT_KEY                   "PLOT_HEIGHT"       
#define  PLOT_PATH_KEY                     "PLOT_PATH"         
#define  PLOT_REFCASE_KEY                  "PLOT_REFCASE"
#define  PLOT_WIDTH_KEY                    "PLOT_WIDTH" 
/*Workflow options are currently not printed.*/
#define  WORKFLOW_JOB_DIRECTORY_KEY        "WORKFLOW_JOB_DIRECTORY"
#define  LOAD_WORKFLOW_KEY                 "LOAD_WORKFLOW"                       
#define  LOAD_WORKFLOW_JOB_KEY             "LOAD_WORKFLOW_JOB"

#define CONFIG_BOOL_STRING( var ) (var) ? "TRUE" : "FALSE"




#ifdef   __cplusplus
}
#endif
#endif
