/*
   Copyright (C) 2013  Statoil ASA, Norway. 
    
   The file 'enkf_workflow_job_test.c' is part of ERT - Ensemble based Reservoir Tool.
    
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
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <ert/util/test_util.h>
#include <ert/util/test_work_area.h>
#include <ert/enkf/ert_test_context.h>

#include <ert/util/util.h>
#include <ert/util/string_util.h>

#include <ert/enkf/enkf_main.h>
#include <ert/enkf/enkf_main_jobs.h>


void test_create_case_job(const char * config_file, const char * job_file) {
  ert_test_context_type * test_context = ert_test_context_alloc("CreateCaseJob" , config_file , NULL);
  stringlist_type * args = stringlist_alloc_new();
  stringlist_append_copy( args , "newly_created_case");
  ert_test_context_install_workflow_job( test_context , "JOB" , job_file );
  test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

  char * new_case = util_alloc_filename( "storage" , "newly_created_case" , NULL);
  test_assert_true(util_is_directory(new_case));
  free(new_case);

  stringlist_free( args );
  ert_test_context_free( test_context );
}

void test_load_results_job(const char * config_file, const char * job_file) {
  ert_test_context_type * test_context = ert_test_context_alloc("LoadResultsJob" , config_file , NULL);
  stringlist_type * args = stringlist_alloc_new();
  ert_test_context_install_workflow_job( test_context , "JOB" , job_file );
  stringlist_append_copy( args , "0,1");
  test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );
  stringlist_free( args );
  ert_test_context_free( test_context );
}

void test_rank_realizations_on_observations_job(const char * config_file, const char * job_file) {
  ert_test_context_type * test_context = ert_test_context_alloc("RankRealizationsOnObservationsJOb" , config_file , NULL);
  stringlist_type * args = stringlist_alloc_new();
  ert_test_context_install_workflow_job( test_context , "JOB" , job_file );

  stringlist_append_copy( args , "NameOfObsRanking1");
  stringlist_append_copy( args , "|");
  stringlist_append_copy( args , "WOPR:*");
  test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

  stringlist_clear(args);
  stringlist_append_copy( args , "NameOfObsRanking2");
  stringlist_append_copy( args, "1-5");
  stringlist_append_copy( args, "55");
  stringlist_append_copy( args , "|");
  stringlist_append_copy( args , "WWCT:*");
  stringlist_append_copy( args , "WOPR:*");
  test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

  stringlist_clear(args);
  stringlist_append_copy( args , "NameOfObsRanking3");
  stringlist_append_copy( args, "5");
  stringlist_append_copy( args, "55");
  stringlist_append_copy( args, "|");
  test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

  stringlist_clear(args);
  stringlist_append_copy( args , "NameOfObsRanking4");
  stringlist_append_copy( args, "1,3,5-10");
  stringlist_append_copy( args, "55");
  test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

  stringlist_clear(args);
  stringlist_append_copy( args , "NameOfObsRanking5");
  test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

  stringlist_clear(args);
  stringlist_append_copy( args , "NameOfObsRanking6");
  stringlist_append_copy( args, "|");
  stringlist_append_copy( args , "UnrecognizableObservation");
  test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

  stringlist_free( args );
  ert_test_context_free( test_context );
}


void test_rank_realizations_on_data_job(const char * config_file, const char * job_file) {
  ert_test_context_type * test_context = ert_test_context_alloc("RankRealizationsOnDataJob" , config_file , NULL);
  stringlist_type * args = stringlist_alloc_new();
  ert_test_context_install_workflow_job( test_context , "JOB" , job_file );

  stringlist_append_copy( args , "NameOfDataRanking");
  stringlist_append_copy( args , "PORO:1,2,3");
  stringlist_append_copy( args , "false");
  stringlist_append_copy( args , "0");
  test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

  stringlist_clear(args);
  stringlist_append_copy( args , "NameOfDataRanking2");
  stringlist_append_copy( args , "PORO:1,2,3");
  stringlist_append_copy( args , "false");
  test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

  stringlist_free( args );
  ert_test_context_free( test_context );
}

  void test_export_ranking(const char * config_file, const char * job_file) {
    ert_test_context_type * test_context = ert_test_context_alloc("ExportRankingJob" , config_file , NULL);
    stringlist_type * args = stringlist_alloc_new();
    ert_test_context_install_workflow_job( test_context , "JOB" , job_file );

    stringlist_append_copy( args , "NameOfDataRanking");
    stringlist_append_copy( args , "/tmp/fileToSaveDataRankingIn.txt");
    test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

    stringlist_clear(args);
    stringlist_append_copy( args , "NameOfObsRanking1");
    stringlist_append_copy( args , "/tmp/fileToSaveObservationRankingIn1.txt");
    test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

    stringlist_clear(args);
    stringlist_append_copy( args , "NameOfObsRanking6");
    stringlist_append_copy( args , "/tmp/fileToSaveObservationRankingIn6.txt");
    test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

    stringlist_free( args );
    ert_test_context_free( test_context );
  }

  void test_init_misfit_table(const char * config_file, const char * job_file) {
    ert_test_context_type * test_context = ert_test_context_alloc("ExportRankingJob" , config_file , NULL);
    stringlist_type * args = stringlist_alloc_new();
    ert_test_context_install_workflow_job( test_context , "JOB" , job_file );

    enkf_main_type * enkf_main = ert_test_context_get_main(test_context);
    enkf_fs_type               * fs              = enkf_main_get_fs(enkf_main);

    misfit_ensemble_type * misfit_ensemble = enkf_fs_get_misfit_ensemble( fs );
    test_assert_false(misfit_ensemble_initialized(misfit_ensemble));

    test_assert_true( ert_test_context_run_worklow_job( test_context , "JOB" , args) );

    test_assert_true(misfit_ensemble_initialized(misfit_ensemble));

    stringlist_free( args );
    ert_test_context_free( test_context );
  }


int main(int argc , const char ** argv) {
  enkf_main_install_SIGNALS();
  
  const char * config_file                  = argv[1];
  const char * job_file_create_case         = argv[2];
  const char * job_file_load_results        = argv[3];
  const char * job_file_observation_ranking = argv[4];
  const char * job_file_data_ranking        = argv[5];
  const char * job_file_ranking_export      = argv[6];
  const char * job_file_init_misfit_table   = argv[7];

  test_create_case_job(config_file, job_file_create_case);
  test_load_results_job(config_file, job_file_load_results);
  test_rank_realizations_on_observations_job(config_file, job_file_observation_ranking);
  test_rank_realizations_on_data_job(config_file, job_file_data_ranking);
  test_export_ranking(config_file, job_file_ranking_export);
  test_init_misfit_table(config_file, job_file_init_misfit_table);



  exit(0);
}
