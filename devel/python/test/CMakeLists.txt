set(NFS_RUNPATH "" CACHE STRING  "Disk area which is shared among cluster nodes and can be used as CWD for LSF/RSH jobs.")
set(RSH_SERVERS "" CACHE STRING  "List of nodes which will be used to test the RSH driver")

set(TEST_SOURCES
    ert_tests/__init__.py
    ert_tests/run_tests.py
    ert_tests/test_ert_import.py
    ert_tests/import_tester.py

    ert_tests/config/__init__.py
    ert_tests/config/test_config.py

    ert_tests/cwrap/__init__.py
    ert_tests/cwrap/test_cwrap.py

    ert_tests/testcase/__init__.py
    ert_tests/testcase/test_testcase.py

    ert_tests/ecl/__init__.py
    ert_tests/ecl/test_deprecation.py
    ert_tests/ecl/test_ecl_default.py
    ert_tests/ecl/test_ecl_file.py
    ert_tests/ecl/test_ecl_submit.py
    ert_tests/ecl/test_fortio.py
    ert_tests/ecl/test_grdecl.py
    ert_tests/ecl/test_grid_statoil.py
    ert_tests/ecl/test_kw.py
    ert_tests/ecl/test_ecl_3dkw.py
    ert_tests/ecl/test_ecl_init_file.py
    ert_tests/ecl/test_legacy_ecl.py
    ert_tests/ecl/test_region.py
    ert_tests/ecl/test_restart.py
    ert_tests/ecl/test_rft.py
    ert_tests/ecl/test_rft_cell.py
    ert_tests/ecl/test_sum.py
    ert_tests/ecl/test_ecl_sum_vector.py
    ert_tests/ecl/test_faults.py
    ert_tests/ecl/test_statoil_faults.py
    ert_tests/ecl/test_fault_blocks.py
    ert_tests/ecl/test_region_definition_statoil.py
    ert_tests/ecl/test_region_definition.py
    ert_tests/ecl/test_npv.py
    ert_tests/ecl/test_indexed_read.py

    ert_tests/enkf/__init__.py
    ert_tests/enkf/test_custom_kw_config_set.py
    ert_tests/enkf/test_ert_context.py
    ert_tests/enkf/test_enkf.py
    ert_tests/enkf/test_enkf_fs.py
    ert_tests/enkf/test_enkf_fs_manager1.py
    ert_tests/enkf/test_enkf_fs_manager2.py
    ert_tests/enkf/test_enkf_library.py
    ert_tests/enkf/test_enkf_load_results_manually.py
    ert_tests/enkf/test_analysis_config.py
    ert_tests/enkf/test_analysis_iter_config.py
    ert_tests/enkf/test_state_map.py
    ert_tests/enkf/test_summary_key_set.py
    ert_tests/enkf/test_summary_key_matcher.py
    ert_tests/enkf/test_time_map.py
    ert_tests/enkf/test_enkf_obs.py
    ert_tests/enkf/test_meas_block.py
    ert_tests/enkf/test_meas_data.py
    ert_tests/enkf/test_obs_data.py
    ert_tests/enkf/test_obs_block.py
    ert_tests/enkf/test_run_arg.py
    ert_tests/enkf/test_runpath_list.py
    ert_tests/enkf/test_update.py

    ert_tests/enkf/plot/test_plot_data.py

    ert_tests/enkf/data/test_custom_kw.py
    ert_tests/enkf/data/test_custom_kw_config.py
    ert_tests/enkf/data/test_gen_data.py
    ert_tests/enkf/data/test_gen_data_config.py
    ert_tests/enkf/data/test_gen_kw.py
    ert_tests/enkf/data/test_gen_kw_config.py

    ert_tests/enkf/export/test_numpy_and_pandas.py
    ert_tests/enkf/export/test_design_matrix.py
    ert_tests/enkf/export/test_export_join.py
    ert_tests/enkf/export/test_gen_kw_collector.py
    ert_tests/enkf/export/test_gen_data_collector.py
    ert_tests/enkf/export/test_misfit_collector.py
    ert_tests/enkf/export/test_summary_collector.py
    ert_tests/enkf/export/test_summary_observation_collector.py

    ert_tests/analysis/__init__.py
    ert_tests/analysis/test_options_enum.py
    ert_tests/analysis/test_analysis_module.py
    ert_tests/analysis/test_rml.py

    ert_tests/job_queue/__init__.py
    ert_tests/job_queue/test_ert_plugin.py
    ert_tests/job_queue/test_ert_script.py
    ert_tests/job_queue/test_function_ert_script.py
    ert_tests/job_queue/test_job_queue.py
    ert_tests/job_queue/test_workflow.py
    ert_tests/job_queue/test_workflow_job.py
    ert_tests/job_queue/test_workflow_joblist.py
    ert_tests/job_queue/test_workflow_runner.py
    ert_tests/job_queue/workflow_common.py

    ert_tests/sched/__init__.py
    ert_tests/sched/test_sched.py

    ert_tests/util/__init__.py
    ert_tests/util/test_ctime.py
    ert_tests/util/test_hash.py
    ert_tests/util/test_lookup_table.py
    ert_tests/util/test_matrix.py
    ert_tests/util/test_stat.py
    ert_tests/util/test_string_list.py
    ert_tests/util/test_vectors.py
    ert_tests/util/test_ui_return.py
    ert_tests/util/test_thread_pool.py
    ert_tests/util/test_work_area.py
    ert_tests/util/test_rng.py

    ert_tests/gui/__init__.py
    ert_tests/gui/test_model.py
    ert_tests/gui/test_observable.py

    ert_tests/gui/plot/test_plot_metrics_tracker.py

    ert_tests/gui/ide/__init__.py
    ert_tests/gui/ide/test_bool_argument.py
    ert_tests/gui/ide/test_configuration_line.py
    ert_tests/gui/ide/test_configuration_line_builder.py
    ert_tests/gui/ide/test_configuration_line_definition.py
    ert_tests/gui/ide/test_configuration_line_parser.py
    ert_tests/gui/ide/test_ert_keywords.py
    ert_tests/gui/ide/test_float_argument.py
    ert_tests/gui/ide/test_integer_argument.py
    ert_tests/gui/ide/test_percent_argument.py
    ert_tests/gui/ide/test_path_argument.py
    ert_tests/gui/ide/test_path_argument.py
    ert_tests/gui/ide/test_proper_name_argument.py
    ert_tests/gui/ide/test_proper_name_format_argument.py
    ert_tests/gui/ide/test_proper_name_format_string_argument.py
    ert_tests/gui/ide/test_range_string_argument.py
    ert_tests/gui/ide/test_tokens.py

    ert_tests/gui/ide/wizards/__init__.py
    ert_tests/gui/ide/wizards/test_tree_structure.py

    ert_tests/geometry/__init__.py
    ert_tests/geometry/test_polyline.py
    ert_tests/geometry/test_intersection.py
    ert_tests/geometry/test_convex_hull.py
    ert_tests/geometry/test_point_in_polygon.py
    ert_tests/geometry/test_polygon_slicing.py


    ert_tests/run/__init__.py
    ert_tests/run/test_run.py

    ert_tests/server/test_server.py
    ert_tests/server/test_socket.py

    ert_tests/well/test_ecl_well.py
    ert_tests/well/test_ecl_well2.py

    ecl_isosurf.py
    ens_config_test.py
    grav_test.py
    grid_test0.py
    job_test.py
    petrel_kw.py
    poly_test.py
    troll_test.py
    ctest_run.py
    ctest_import.py
)

if (ERT_BUILD_GUI) 
   LIST(APPEND ${TEST_SOURCES} ert_tests/test_ert_gui_import.py)
endif()

add_python_package("python.tests" "${PYTHON_INSTALL_PREFIX}/test" "${TEST_SOURCES}" False)

function (addPythonTest TEST_NAME TEST_CLASS)
    add_test( NAME ${TEST_NAME}
              WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
              COMMAND ctest_run.py ${PROJECT_BINARY_DIR}/${PYTHON_INSTALL_PREFIX} ${TEST_CLASS}
            )


endfunction(addPythonTest)

# The test data is located in the current source directory; that is
# the reason we set that as the working directory for the test
# run. The module to import should then be in the default python
# module search path (i.e. current directory), whereas the location of
# the actual ert package is given by the first argument to ctest.


add_test( NAME python.tests.ert.import
          WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/${PYTHON_INSTALL_PREFIX}
          COMMAND python test/ctest_import.py ${PROJECT_BINARY_DIR}/${PYTHON_INSTALL_PREFIX} "ert")



if (ERT_BUILD_GUI) 
   add_test( NAME python.tests.ert_gui.import
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/${PYTHON_INSTALL_PREFIX}
             COMMAND python test/ctest_import.py ${PROJECT_BINARY_DIR}/${PYTHON_INSTALL_PREFIX} "ert_gui")
endif()

#TESTCASE
addPythonTest(python.tests.ert.testcase.testcase ert_tests.testcase.test_testcase.TestTestCase)

#UTIL
addPythonTest(python.tests.ert.util.stringlist ert_tests.util.test_string_list.StringListTest)
addPythonTest(python.tests.ert.util.ui_return ert_tests.util.test_ui_return.UIReturnTest)
addPythonTest(python.tests.ert.util.tvector ert_tests.util.test_vectors.UtilTest)
addPythonTest(python.tests.ert.util.ctime ert_tests.util.test_ctime.CTimeTest)
addPythonTest(python.tests.ert.util.hash ert_tests.util.test_hash.HashTest)
addPythonTest(python.tests.ert.util.work_area ert_tests.util.test_work_area.WorkAreaTest)
addPythonTest(python.tests.ert.util.lookup_table ert_tests.util.test_lookup_table.LookupTableTest )
addPythonTest(python.tests.ert.util.matrix ert_tests.util.test_matrix.MatrixTest )
addPythonTest(python.tests.ert.util.stat ert_tests.util.test_stat.StatTest )
addPythonTest(python.tests.ert.util.rng ert_tests.util.test_rng.RngTest )

#add_test( NAME python.tests.ert.util.latex
#          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#          COMMAND ctest_run.py ${PROJECT_BINARY_DIR}/${PYTHON_INSTALL_PREFIX} ert_tests.util.test_latex.LatexTest )


#ENKF
addPythonTest(python.tests.ert.enkf.enkf ert_tests.enkf.test_enkf.EnKFTest)
addPythonTest(python.tests.ert.enkf.enkf_obs ert_tests.enkf.test_enkf_obs.EnKFObsTest)
addPythonTest(python.tests.ert.enkf.meas_block ert_tests.enkf.test_meas_block.MeasBlockTest)
addPythonTest(python.tests.ert.enkf.meas_data ert_tests.enkf.test_meas_data.MeasDataTest)
addPythonTest(python.tests.ert.enkf.obs_data ert_tests.enkf.test_obs_data.ObsDataTest)
addPythonTest(python.tests.ert.enkf.obs_block ert_tests.enkf.test_obs_block.ObsBlockTest)
addPythonTest(python.tests.ert.enkf.enkf_load_results_manually ert_tests.enkf.test_enkf_load_results_manually.LoadResultsManuallyTest)
addPythonTest(python.tests.ert.enkf.enkf_fs ert_tests.enkf.test_enkf_fs.EnKFFSTest)
addPythonTest(python.tests.ert.enkf.ert_log ert_tests.enkf.test_ert_log.ErtLogTest)


addPythonTest(python.tests.ert.enkf.enkf_fs_manager1 ert_tests.enkf.test_enkf_fs_manager1.EnKFFSManagerTest1)
addPythonTest(python.tests.ert.enkf.enkf_fs_manager2 ert_tests.enkf.test_enkf_fs_manager2.EnKFFSManagerTest2)
addPythonTest(python.tests.ert.enkf.enkf_library ert_tests.enkf.test_enkf_library.EnKFLibraryTest)
addPythonTest(python.tests.ert.enkf.analysis_config ert_tests.enkf.test_analysis_config.AnalysisConfigTest)
addPythonTest(python.tests.ert.enkf.analysis_iter_config ert_tests.enkf.test_analysis_iter_config.AnalysisIterConfigTest)
addPythonTest(python.tests.ert.enkf.state_map ert_tests.enkf.test_state_map.StateMapTest)
addPythonTest(python.tests.ert.enkf.summary_key_set ert_tests.enkf.test_summary_key_set.SummaryKeySetTest)
addPythonTest(python.tests.ert.enkf.custom_kw_config_set ert_tests.enkf.test_custom_kw_config_set.CustomKWConfigSetTest)
addPythonTest(python.tests.ert.enkf.summary_key_matcher ert_tests.enkf.test_summary_key_matcher.SummaryKeyMatcherTest)
addPythonTest(python.tests.ert.enkf.time_map ert_tests.enkf.test_time_map.TimeMapTest)
addPythonTest(python.tests.ert.enkf.ecl_config ert_tests.enkf.test_ecl_config.EclConfigTest)
addPythonTest(python.tests.ert.enkf.plot.plot_block_data ert_tests.enkf.plot.test_plot_data.PlotDataTest)
addPythonTest(python.tests.ert.enkf.ert_test_context ert_tests.enkf.test_ert_context.ErtTestContextTest)
addPythonTest(python.tests.ert.enkf.run_arg ert_tests.enkf.test_run_arg.RunArg)
addPythonTest(python.tests.ert.enkf.runpath_list ert_tests.enkf.test_runpath_list.RunpathListTest)


#enkf.data
addPythonTest(python.tests.ert.enkf.data.custom_kw ert_tests.enkf.data.test_custom_kw.CustomKWTest)
addPythonTest(python.tests.ert.enkf.data.custom_kw_config ert_tests.enkf.data.test_custom_kw_config.CustomKWConfigTest)
addPythonTest(python.tests.ert.enkf.data.gen_data ert_tests.enkf.data.test_gen_data.GenDataTest)
addPythonTest(python.tests.ert.enkf.data.gen_data_config ert_tests.enkf.data.test_gen_data_config.GenDataConfigTest)
addPythonTest(python.tests.ert.enkf.data.gen_kw ert_tests.enkf.data.test_gen_kw.GenKwTest)
addPythonTest(python.tests.ert.enkf.data.gen_kw_config ert_tests.enkf.data.test_gen_kw_config.GenKwConfigTest)


#enkf.export
addPythonTest(python.tests.ert.enkf.export.numpy_and_pandas ert_tests.enkf.export.test_numpy_and_pandas.NumpyAndPandasTest)
addPythonTest(python.tests.ert.enkf.export.design_matrix ert_tests.enkf.export.test_design_matrix.DesignMatrixTest)
addPythonTest(python.tests.ert.enkf.export.export_join ert_tests.enkf.export.test_export_join.ExportJoinTest)
addPythonTest(python.tests.ert.enkf.export.gen_kw_collector ert_tests.enkf.export.test_gen_kw_collector.GenKwCollectorTest)
addPythonTest(python.tests.ert.enkf.export.gen_data_collector ert_tests.enkf.export.test_gen_data_collector.GenDataCollectorTest)
addPythonTest(python.tests.ert.enkf.export.arg_loader ert_tests.enkf.export.test_arg_loader.ArgLoaderTest)
addPythonTest(python.tests.ert.enkf.export.misfit_collector ert_tests.enkf.export.test_misfit_collector.MisfitCollectorTest)
addPythonTest(python.tests.ert.enkf.export.summary_collector ert_tests.enkf.export.test_summary_collector.SummaryCollectorTest)
addPythonTest(python.tests.ert.enkf.export.summary_observation_collector ert_tests.enkf.export.test_summary_observation_collector.SummaryObservationCollectorTest)
addPythonTest(python.tests.ert.enkf.update ert_tests.enkf.test_update.UpdateTest)



#GUI
if (ERT_BUILD_GUI) 
   addPythonTest(python.tests.ert_gui.models ert_tests.gui.test_model.ModelTest)
   addPythonTest(python.tests.ert_gui.models.observable ert_tests.gui.test_observable.ObservableTest)
   addPythonTest(python.tests.ert_gui.plot.plot_metrics_tracker ert_tests.gui.plot.test_plot_metrics_tracker.PlotMetricTrackerTest)
   addPythonTest(python.tests.ert_gui.ide.bool_argument ert_tests.gui.ide.test_bool_argument.BoolArgumentTest)
   addPythonTest(python.tests.ert_gui.ide.integer_argument ert_tests.gui.ide.test_integer_argument.IntegerArgumentTest)
   addPythonTest(python.tests.ert_gui.ide.float_argument ert_tests.gui.ide.test_float_argument.FloatArgumentTest)
   addPythonTest(python.tests.ert_gui.ide.path_argument ert_tests.gui.ide.test_path_argument.PathArgumentTest)
   addPythonTest(python.tests.ert_gui.ide.proper_name_argument ert_tests.gui.ide.test_proper_name_argument.ProperNameArgumentTest)
   addPythonTest(python.tests.ert_gui.ide.proper_name_format_argument ert_tests.gui.ide.test_proper_name_format_argument.ProperNameFormatArgumentTest)
   addPythonTest(python.tests.ert_gui.ide.proper_name_format_string_argument ert_tests.gui.ide.test_proper_name_format_string_argument.ProperNameFormatStringArgumentTest)
   addPythonTest(python.tests.ert_gui.ide.range_string_argument ert_tests.gui.ide.test_range_string_argument.RangeStringArgumentTest)
   addPythonTest(python.tests.ert_gui.ide.test_configuration_line ert_tests.gui.ide.test_configuration_line.ConfigurationLineTest)
   addPythonTest(python.tests.ert_gui.ide.test_configuration_line_builder ert_tests.gui.ide.test_configuration_line_builder.ConfigurationLineBuilderTest)
   addPythonTest(python.tests.ert_gui.ide.test_configuration_line_definition ert_tests.gui.ide.test_configuration_line_definition.ConfigurationLineDefinitionTest)
   addPythonTest(python.tests.ert_gui.ide.test_configuration_line_parser ert_tests.gui.ide.test_configuration_line_parser.ConfigurationLineParserTest)
   addPythonTest(python.tests.ert_gui.ide.ert_keywords ert_tests.gui.ide.test_ert_keywords.ErtKeywordTest)
   addPythonTest(python.tests.ert_gui.ide.tokens ert_tests.gui.ide.test_tokens.TokenTest)
   addPythonTest(python.tests.ert_gui.ide.wizards.tree_structure ert_tests.gui.ide.wizards.test_tree_structure.TreeStructureTest)
endif()

#ECL


addPythonTest(python.tests.ert.ecl.ecl_default ert_tests.ecl.test_ecl_default.EclDefaultTest )
addPythonTest(python.tests.ert.ecl.ecl_file ert_tests.ecl.test_ecl_file.EclFileTest)
addPythonTest(python.tests.ert.ecl.ecl_queue_LOCAL ert_tests.ecl.test_ecl_submit.LocalSubmitTest  )

if (NOT ${NFS_RUNPATH} STREQUAL "")
   if (ERT_LSF_SUBMIT_TEST)
      addPythonTest(python.tests.ert.ecl.ecl_queue_LSF ert_tests.ecl.test_ecl_submit.LSFSubmitTest ${NFS_RUNPATH} )
      set_property( TEST python.tests.ert.ecl.ecl_queue_LSF     PROPERTY LABELS Python:StatoilData:Slow )
   endif()

   if (NOT ${RSH_SERVERS} STREQUAL "")
      addPythonTest(python.tests.ert.ecl.ecl_queue_RSH ert_tests.ecl.test_ecl_submit.RSHSubmitTest ${NFS_RUNPATH} ${RSH_SERVERS})
      set_property( TEST python.tests.ert.ecl.ecl_queue_RSH     PROPERTY LABELS Python:StatoilData:Slow )
   endif()
endif()


addPythonTest(python.tests.ert.ecl.fortio ert_tests.ecl.test_fortio.FortIOTest)
addPythonTest(python.tests.ert.ecl.ecl_grdecl ert_tests.ecl.test_grdecl.GRDECLTest )
addPythonTest(python.tests.ert.ecl.ecl_grid_statoil ert_tests.ecl.test_grid_statoil.GridTest )
addPythonTest(python.tests.ert.ecl.ecl_grid ert_tests.ecl.test_grid.GridTest )
addPythonTest(python.tests.ert.ecl.ecl_kw ert_tests.ecl.test_kw.KWTest )
addPythonTest(python.tests.ert.ecl.ecl_3dkw ert_tests.ecl.test_ecl_3dkw.Ecl3DKWTest )
addPythonTest(python.tests.ert.ecl.ecl_init_file ert_tests.ecl.test_ecl_init_file.InitFileTest )
addPythonTest(python.tests.ert.ecl.ecl_restart_file ert_tests.ecl.test_ecl_restart_file.RestartFileTest )
addPythonTest(python.tests.ert.ecl.legacy_ecl ert_tests.ecl.test_legacy_ecl.LegacyEclTest)
addPythonTest(python.tests.ert.ecl.ecl_restart ert_tests.ecl.test_restart.RestartTest)
addPythonTest(python.tests.ert.ecl.ecl_region ert_tests.ecl.test_region.RegionTest)
addPythonTest(python.tests.ert.ecl.ecl_rft ert_tests.ecl.test_rft.RFTTest)
addPythonTest(python.tests.ert.ecl.ecl_rft_cell ert_tests.ecl.test_rft_cell.RFTCellTest)
addPythonTest(python.tests.ert.ecl.ecl_sum ert_tests.ecl.test_sum.SumTest)
addPythonTest(python.tests.ert.ecl.ecl_sum_vector ert_tests.ecl.test_ecl_sum_vector.EclSumVectorTest)
addPythonTest(python.tests.ert.ecl.ecl_sum_time_range ert_tests.ecl.test_ecl_sum.EclSumTest)
addPythonTest(python.tests.ert.ecl.layer ert_tests.ecl.test_layer.LayerTest )
addPythonTest(python.tests.ert.ecl.faults ert_tests.ecl.test_faults.FaultTest )
addPythonTest(python.tests.ert.ecl.fault_blocks ert_tests.ecl.test_fault_blocks.FaultBlockTest )
addPythonTest(python.tests.ert.ecl.region_definition_statoil ert_tests.ecl.test_region_definition_statoil.RegionDefinitionTest )
addPythonTest(python.tests.ert.ecl.region_definition ert_tests.ecl.test_region_definition.RegionDefinitionTest )
addPythonTest(python.tests.ert.ecl.fault_blocks_statoil ert_tests.ecl.test_fault_blocks_statoil.FaultBlockTest )
addPythonTest(python.tests.ert.ecl.ecl_npv ert_tests.ecl.test_npv.NPVTest)
addPythonTest(python.tests.ert.ecl.statoil_faults ert_tests.ecl.test_statoil_faults.StatoilFaultTest )
addPythonTest(python.tests.ert.ecl.ecl_deprecation ert_tests.ecl.test_deprecation.DeprecationTest )
addPythonTest(python.tests.ert.ecl.indexed_read ert_tests.ecl.test_indexed_read.EclIndexedReadTest )

#SCHED
addPythonTest(python.tests.ert.sched.sched ert_tests.sched.test_sched.SchedFileTest)

#JOB QUEUE
addPythonTest(python.tests.ert.job_queue.job_status_type_enum ert_tests.job_queue.test_job_queue.JobQueueTest)
addPythonTest(python.tests.ert.job_queue.workflow_job ert_tests.job_queue.test_workflow_job.WorkflowJobTest)
addPythonTest(python.tests.ert.job_queue.workflow ert_tests.job_queue.test_workflow.WorkflowTest)
addPythonTest(python.tests.ert.job_queue.workflow_joblist ert_tests.job_queue.test_workflow_joblist.WorkflowJoblistTest)
addPythonTest(python.tests.ert.job_queue.ert_plugin ert_tests.job_queue.test_ert_plugin.ErtPluginTest)
addPythonTest(python.tests.ert.job_queue.ert_script ert_tests.job_queue.test_ert_script.ErtScriptTest)
addPythonTest(python.tests.ert.job_queue.function_ert_script ert_tests.job_queue.test_function_ert_script.FunctionErtScriptTest)
addPythonTest(python.tests.ert.job_queue.workflow_runner ert_tests.job_queue.test_workflow_runner.WorkflowRunnerTest)


#CONFIG
addPythonTest(python.tests.ert.config ert_tests.config.test_config.ConfigTest)


#CWRAP
addPythonTest(python.tests.ert.cwrap ert_tests.cwrap.test_cwrap.CWRapTest)
addPythonTest(python.tests.ert.cwrap.basecclass ert_tests.cwrap.test_basecclass.BaseCClassTest)
addPythonTest(python.tests.ert.cwrap.basecenum ert_tests.cwrap.test_basecenum.BaseCEnumTest)
addPythonTest(python.tests.ert.cwrap.basecvalue ert_tests.cwrap.test_basecvalue.BaseCValueTest)


#TESTRUN
addPythonTest(python.tests.ert.test_run ert_tests.run.test_run.RunTest )

#ANALYSIS
addPythonTest(python.tests.ert.analysis.enums ert_tests.analysis.test_options_enum.AnalysisOptionsEnumTest)
addPythonTest(python.tests.ert.analysis.analysis_module ert_tests.analysis.test_analysis_module.AnalysisModuleTest)
addPythonTest(python.tests.ert.analysis.rml ert_tests.analysis.test_rml.RMLTest)



#GEOMETRY
addPythonTest(python.tests.ert.geometry.polyline ert_tests.geometry.test_polyline.PolylineTest)
addPythonTest(python.tests.ert.geometry.intersection ert_tests.geometry.test_intersection.IntersectionTest)
addPythonTest(python.tests.ert.geometry.convex_hull ert_tests.geometry.test_convex_hull.ConvexHullTest)
addPythonTest(python.tests.ert.geometry.point_in_polygon ert_tests.geometry.test_point_in_polygon.PointInPolygonTest)
addPythonTest(python.tests.ert.geometry.polygon_slicing ert_tests.geometry.test_polygon_slicing.PolygonSlicingTest)


#SERVER
addPythonTest(python.tests.ert.server.socket ert_tests.server.test_socket.SocketTest)
addPythonTest(python.tests.ert.server.server ert_tests.server.test_server.ServerTest)


# ECL_WELL
addPythonTest(python.tests.ert.well.ecl_well ert_tests.well.test_ecl_well.EclWellTest)
addPythonTest(python.tests.ert.well.ecl_well2 ert_tests.well.test_ecl_well2.EclWellTest2)


set_property( TEST python.tests.ert.sched.sched           PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.fortio            PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_grdecl        PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_restart       PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_region        PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_file          PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_init_file     PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_restart_file  PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_queue_LOCAL   PROPERTY LABELS Python:StatoilData:Slow )
set_property( TEST python.tests.ert.ecl.ecl_sum           PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_npv           PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_kw            PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_rft           PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_sum_vector    PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.indexed_read      PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_grid_statoil  PROPERTY LABELS Python:StatoilData:Slow )
set_property( TEST python.tests.ert.ecl.ecl_deprecation   PROPERTY LABELS Python )
#set_property( TEST python.tests.import_local             PROPERTY LABELS Python:StatoilBuild )
set_property( TEST python.tests.ert.well.ecl_well         PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.well.ecl_well2        PROPERTY LABELS Python:StatoilData )

set_property( TEST python.tests.ert.test_run              PROPERTY LABELS Python)
set_property( TEST python.tests.ert.import                PROPERTY LABELS Python)
set_property( TEST python.tests.ert.util.stringlist       PROPERTY LABELS Python )
set_property( TEST python.tests.ert.util.tvector          PROPERTY LABELS Python )
set_property( TEST python.tests.ert.ecl.faults            PROPERTY LABELS Python )
set_property( TEST python.tests.ert.ecl.ecl_grid          PROPERTY LABELS Python )

set_property( TEST python.tests.ert.ecl.region_definition_statoil PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.fault_blocks_statoil  PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.statoil_faults        PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.enkf                 PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.ecl_config           PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.enkf_fs              PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.enkf_fs_manager1     PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.enkf_fs_manager2     PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.enkf_obs             PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.enkf_load_results_manually     PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.ert_test_context     PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.plot.plot_block_data PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.data.gen_data_config PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.data.gen_data        PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.update               PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.summary_key_set      PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.data.gen_kw_config   PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.export.design_matrix     PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.export.export_join       PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.export.gen_kw_collector  PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.export.gen_data_collector  PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.export.arg_loader          PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.export.misfit_collector  PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.export.summary_collector PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.ecl.ecl_sum_time_range    PROPERTY LABELS Python:StatoilData )
set_property( TEST python.tests.ert.enkf.export.summary_observation_collector PROPERTY LABELS Python:StatoilData)

set_property( TEST python.tests.ert.enkf.data.custom_kw PROPERTY ENVIRONMENT "ERT_SITE_CONFIG=${SITE_CONFIG_FILE}")

if (ERT_BUILD_GUI)
   set_property( TEST python.tests.ert_gui.import         PROPERTY ENVIRONMENT "ERT_SHARE_PATH=${PROJECT_SOURCE_PATH}/share")
   set_property( TEST python.tests.ert_gui.import         PROPERTY LABELS Python)
   set_property( TEST python.tests.ert_gui.models         PROPERTY LABELS Python)
   set_property( TEST python.tests.ert_gui.models.observable PROPERTY LABELS Python)
endif()
