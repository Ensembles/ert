add_executable( ecl_alloc_grid_dxv_dyv_dzv ecl_alloc_grid_dxv_dyv_dzv.c )
target_link_libraries( ecl_alloc_grid_dxv_dyv_dzv ecl test_util )
add_test( ecl_alloc_grid_dxv_dyv_dzv  ${EXECUTABLE_OUTPUT_PATH}/ecl_alloc_grid_dxv_dyv_dzv )

add_executable( ecl_coarse_test ecl_coarse_test.c )
target_link_libraries( ecl_coarse_test ecl test_util )
add_test( ecl_coarse_test  ${EXECUTABLE_OUTPUT_PATH}/ecl_coarse_test  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/LGCcase/LGC_TESTCASE2 )


add_executable( ecl_restart_test ecl_restart_test.c )
target_link_libraries( ecl_restart_test ecl test_util )
add_test( ecl_restart_test ${EXECUTABLE_OUTPUT_PATH}/ecl_restart_test ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.UNRST )

add_executable( ecl_kw_init ecl_kw_init.c )
target_link_libraries( ecl_kw_init ecl test_util )
add_test( ecl_kw_init ${EXECUTABLE_OUTPUT_PATH}/ecl_kw_init  )

add_executable( ecl_kw_fread ecl_kw_fread.c )
target_link_libraries( ecl_kw_fread ecl test_util )
add_test( ecl_kw_fread ${EXECUTABLE_OUTPUT_PATH}/ecl_kw_fread  )

add_executable( ecl_nnc_export ecl_nnc_export.c )
target_link_libraries( ecl_nnc_export ecl test_util )
add_test (ecl_nnc_export1 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_export  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE TRUE)
add_test (ecl_nnc_export2 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_export  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/10kcase/TEST10K_FLT_LGR_NNC TRUE)
add_test (ecl_nnc_export3 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_export  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Troll/MSW_LGR/2BRANCHES-CCEWELLPATH-NEW-SCH-TUNED-AR3 TRUE)
add_test (ecl_nnc_export4 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_export  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/DualPoro/DUAL_DIFF TRUE)
add_test (ecl_nnc_export5 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_export  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/DualPoro/DUALPORO TRUE)
add_test (ecl_nnc_export6 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_export  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/nestedLGRcase/TESTCASE_NESTEDLGR TRUE)
add_test (ecl_nnc_export7 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_export  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/TYRIHANS/BASE20150218_MULTFLT FALSE)

add_executable( ecl_nnc_export_get_tran ecl_nnc_export_get_tran.c )
target_link_libraries( ecl_nnc_export_get_tran ecl test_util )
add_test (ecl_nnc_export_get_tran ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_export_get_tran  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Troll/MSW_LGR/2BRANCHES-CCEWELLPATH-NEW-SCH-TUNED-AR3)

add_executable( ecl_valid_basename ecl_valid_basename.c )
target_link_libraries( ecl_valid_basename ecl test_util )
add_test( ecl_valid_basename ${EXECUTABLE_OUTPUT_PATH}/ecl_valid_basename)

add_executable( ecl_util_make_date_no_shift ecl_util_make_date_no_shift.c )
target_link_libraries( ecl_util_make_date_no_shift ecl test_util )
add_test( ecl_util_make_date_no_shift ${EXECUTABLE_OUTPUT_PATH}/ecl_util_make_date_no_shift )

add_executable( ecl_util_make_date_shift ecl_util_make_date_shift.c )
target_link_libraries( ecl_util_make_date_shift ecl test_util )
add_test( ecl_util_make_date_shift ${EXECUTABLE_OUTPUT_PATH}/ecl_util_make_date_shift )

add_executable( ecl_sum_case_exists ecl_sum_case_exists.c )
target_link_libraries( ecl_sum_case_exists ecl test_util )
add_test( ecl_sum_case_exists ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_case_exists 
          ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE
          ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/GurbatSummary/missingHe        ader/ECLIPSE  
          ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/GurbatSummary/missingData/ECLIPSE )
          

add_executable( ecl_grid_lgr_name ecl_grid_lgr_name.c )
target_link_libraries( ecl_grid_lgr_name ecl test_util )
set_target_properties( ecl_grid_lgr_name PROPERTIES COMPILE_FLAGS "-Werror")                                    
add_test( ecl_grid_lgr_name ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_lgr_name  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Troll/MSW_LGR/2BRANCHES-CCEWELLPATH-NEW-SCH-TUNED-AR3.EGRID)

add_executable( ecl_grid_create ecl_grid_create.c )
target_link_libraries( ecl_grid_create ecl test_util )
add_test( ecl_grid_create ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_create )

add_executable( ecl_region ecl_region.c )
target_link_libraries( ecl_region ecl test_util )
add_test( ecl_region ${EXECUTABLE_OUTPUT_PATH}/ecl_region ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID )

add_executable( ecl_grid_fwrite ecl_grid_fwrite.c )
target_link_libraries( ecl_grid_fwrite ecl test_util )
add_test( ecl_grid_fwrite ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_fwrite ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID )

add_executable( ecl_grid_cell_contains ecl_grid_cell_contains.c )
target_link_libraries( ecl_grid_cell_contains ecl test_util )
add_test( ecl_grid_cell_contains1 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_cell_contains 1 )
add_test( ecl_grid_cell_contains2 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_cell_contains 2 ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID )
add_test( ecl_grid_cell_contains3 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_cell_contains 3 ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/FF12/FF12_2013B2.EGRID )

add_test( ecl_grid_cell_contains4 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_cell_contains 4 ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Brazil/R3_ICD.EGRID )

add_executable( ecl_tetrahedron_contains ecl_tetrahedron_contains.c )
target_link_libraries( ecl_tetrahedron_contains ecl test_util )
add_test( ecl_tetrahedron_contains1 ${EXECUTABLE_OUTPUT_PATH}/ecl_tetrahedron_contains)

add_executable( ecl_grid_cell_volume ecl_grid_cell_volume.c )
target_link_libraries( ecl_grid_cell_volume ecl test_util )
add_test( ecl_grid_cell_volume1 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_cell_volume)
add_test( ecl_grid_cell_volume2 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_cell_volume ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID )
add_test( ecl_grid_cell_volume3 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_cell_volume ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Heidrun/Summary/FF12_2013B3_CLEAN_RS.EGRID )

add_executable( ecl_region2region ecl_region2region_test.c )
target_link_libraries( ecl_region2region ecl test_util )
add_test( ecl_region2region ${EXECUTABLE_OUTPUT_PATH}/ecl_region2region ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/R2R/R2R.SMSPEC )

add_executable( ecl_grid_case ecl_grid_case.c )
target_link_libraries( ecl_grid_case ecl test_util )
add_test( ecl_grid_case ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_case ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE )


add_executable( ecl_lgr_test ecl_lgr_test.c )
target_link_libraries( ecl_lgr_test ecl test_util )
add_test( ecl_lgr_test1    ${EXECUTABLE_OUTPUT_PATH}/ecl_lgr_test     ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/10kcase/TEST10K_FLT_LGR_NNC.EGRID)
add_test( ecl_lgr_test2    ${EXECUTABLE_OUTPUT_PATH}/ecl_lgr_test     ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/10kcase/TEST10K_FLT_LGR_NNC.GRID)
add_test( ecl_lgr_test3    ${EXECUTABLE_OUTPUT_PATH}/ecl_lgr_test     ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Troll/MSW_LGR/2BRANCHES-CCEWELLPATH-NEW-SCH-TUNED-AR3.EGRID )


add_executable( ecl_grid_simple ecl_grid_simple.c )
target_link_libraries( ecl_grid_simple ecl test_util )
add_test( ecl_grid_simple ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_simple  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID )


add_executable( ecl_grid_DEPTHZ ecl_grid_DEPTHZ.c )
target_link_libraries( ecl_grid_DEPTHZ ecl test_util )
add_test( ecl_grid_DEPTHZ ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_DEPTHZ )

add_executable( ecl_grid_reset_actnum ecl_grid_reset_actnum.c )
target_link_libraries( ecl_grid_reset_actnum ecl test_util )
add_test( ecl_grid_reset_actnum ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_reset_actnum )

add_executable( ecl_grid_export ecl_grid_export.c )
add_executable( ecl_grid_corner ecl_grid_corner.c )
target_link_libraries( ecl_grid_corner ecl test_util )
add_test( ecl_grid_corner ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_corner )


target_link_libraries( ecl_grid_export ecl test_util )
add_test( ecl_grid_export ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_export  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID )

add_executable( ecl_grid_volume ecl_grid_volume.c )
target_link_libraries( ecl_grid_volume ecl test_util )
add_test( ecl_grid_volume1 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_volume  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE )
add_test( ecl_grid_volume2 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_volume  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/VolumeTest/TEST1 )

# The grid volume test fails miserably on the test case given as example three; looking at
# the failures one could actually suspect that the ECLIPSE algorithm for PORV calculations
# has been different in this file - i.e. that the absolute value of the individual
# tetrahedron parts have been taken during the sum, and not at the end. At least the ert
# algorithm gets volumes ~ 0 whereas ECLIPSE reports ~10^9 for the same cell.
# add_test( ecl_grid_volume3 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_volume  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Heidrun/Summary/FF12_2013B3_CLEAN_RS)

add_executable( ecl_grid_dims ecl_grid_dims.c )
target_link_libraries( ecl_grid_dims ecl test_util )

add_test( ecl_grid_dims0 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_dims  )
add_test( ecl_grid_dims1 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_dims  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.INIT )
add_test( ecl_grid_dims2 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_dims  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.GRID  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.INIT)
add_test( ecl_grid_dims3 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_dims  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID )
add_test( ecl_grid_dims4 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_dims  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.GRID  ) 
add_test( ecl_grid_dims5 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_dims  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/AmalgLGRcase/TESTCASE_AMALG_LGR.EGRID )



add_executable( ecl_nnc_test ecl_nnc_test.c )
target_link_libraries( ecl_nnc_test ecl test_util )
add_test (ecl_nnc_test1 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_test  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID )
add_test (ecl_nnc_test2 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_test  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/10kcase/TEST10K_FLT_LGR_NNC.EGRID )
add_test (ecl_nnc_test3 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_test  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Troll/MSW_LGR/2BRANCHES-CCEWELLPATH-NEW-SCH-TUNED-AR3.EGRID)
add_test (ecl_nnc_test4 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_test  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/DualPoro/DUAL_DIFF.EGRID )
add_test (ecl_nnc_test5 ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_test  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/nestedLGRcase/TESTCASE_NESTEDLGR.EGRID)

add_executable( ecl_nnc_info_test ecl_nnc_info_test.c )
target_link_libraries( ecl_nnc_info_test ecl test_util )
add_test (ecl_nnc_info_test ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_info_test )


add_executable( ecl_nnc_vector ecl_nnc_vector.c )
target_link_libraries( ecl_nnc_vector ecl test_util )
add_test(ecl_nnc_vector ${EXECUTABLE_OUTPUT_PATH}/ecl_nnc_vector )

add_executable( ecl_point ecl_point.c )
target_link_libraries( ecl_point ecl test_util )
add_test(ecl_point ${EXECUTABLE_OUTPUT_PATH}/ecl_point )

add_executable( ecl_layer ecl_layer.c )
target_link_libraries( ecl_layer ecl test_util )
add_test(ecl_layer ${EXECUTABLE_OUTPUT_PATH}/ecl_layer )


add_executable( ecl_layer_statoil ecl_layer_statoil.c )
target_link_libraries( ecl_layer_statoil ecl test_util )
add_test(ecl_layer_statoil ${EXECUTABLE_OUTPUT_PATH}/ecl_layer_statoil ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Mariner/MARINER.EGRID ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Mariner/faultblock.grdecl)  


add_executable( ecl_kw_grdecl ecl_kw_grdecl.c )
target_link_libraries( ecl_kw_grdecl ecl test_util )
add_test( ecl_kw_grdecl ${EXECUTABLE_OUTPUT_PATH}/ecl_kw_grdecl )

add_executable( ecl_kw_equal ecl_kw_equal.c )
target_link_libraries( ecl_kw_equal ecl test_util )
add_test( ecl_kw_equal ${EXECUTABLE_OUTPUT_PATH}/ecl_kw_equal )

add_executable( ecl_kw_cmp_string ecl_kw_cmp_string.c )
target_link_libraries( ecl_kw_cmp_string ecl test_util )
add_test( ecl_kw_cmp_string ${EXECUTABLE_OUTPUT_PATH}/ecl_kw_cmp_string )


add_executable( ecl_dualp ecl_dualp.c )
target_link_libraries( ecl_dualp ecl test_util )
add_test( ecl_dualp ${EXECUTABLE_OUTPUT_PATH}/ecl_dualp  ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/LGCcase/LGC_TESTCASE2 )

add_executable( ecl_util_month_range ecl_util_month_range.c )
target_link_libraries( ecl_util_month_range ecl test_util )
add_test( ecl_util_month_range ${EXECUTABLE_OUTPUT_PATH}/ecl_util_month_range  )

add_executable( ecl_sum_test ecl_sum_test.c )
target_link_libraries( ecl_sum_test ecl test_util )
add_test( ecl_sum_test ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_test ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE )

add_executable( ecl_sum_report_step_equal ecl_sum_report_step_equal.c )
target_link_libraries( ecl_sum_report_step_equal ecl test_util )
add_test( ecl_sum_report_step_equal1 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_equal ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Snorre/SNORRE FALSE)
add_test( ecl_sum_report_step_equal2 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_equal ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE TRUE)
add_test( ecl_sum_report_step_equal3 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_equal ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/modGurbat/extraMinistep/ECLIPSE TRUE)
add_test( ecl_sum_report_step_equal4 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_equal ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/modGurbat/short/ECLIPSE FALSE)
add_test( ecl_sum_report_step_equal5 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_equal ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/modGurbat/enkf/ECLIPSE FALSE)
add_test( ecl_sum_report_step_equal6 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_equal ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Snorre/SNORRE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Snorre2/SNORRE2 FALSE)

add_executable( ecl_sum_report_step_compatible ecl_sum_report_step_compatible.c )
target_link_libraries( ecl_sum_report_step_compatible ecl test_util )
add_test( ecl_sum_report_step_compatible1 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_compatible ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Snorre/SNORRE FALSE)
add_test( ecl_sum_report_step_compatible2 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_compatible ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE TRUE)
add_test( ecl_sum_report_step_compatible3 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_compatible ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/modGurbat/extraMinistep/ECLIPSE TRUE)
add_test( ecl_sum_report_step_compatible4 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_compatible ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/modGurbat/short/ECLIPSE TRUE)
add_test( ecl_sum_report_step_compatible5 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_compatible ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/modGurbat/enkf/ECLIPSE TRUE)
add_test( ecl_sum_report_step_compatible6 ${EXECUTABLE_OUTPUT_PATH}/ecl_sum_report_step_equal ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Snorre/SNORRE ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Snorre2/SNORRE2 FALSE)

add_executable( ecl_fortio ecl_fortio.c )
target_link_libraries( ecl_fortio  ecl test_util )
add_test( ecl_fortio ${EXECUTABLE_OUTPUT_PATH}/ecl_fortio ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.UNRST )

add_executable( ecl_file ecl_file.c )
target_link_libraries( ecl_file  ecl test_util )
add_test( ecl_file ${EXECUTABLE_OUTPUT_PATH}/ecl_file ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.UNRST ECLIPSE.UNRST)

add_executable( ecl_fmt ecl_fmt.c )
target_link_libraries( ecl_fmt  ecl test_util )
add_test( ecl_fmt ${EXECUTABLE_OUTPUT_PATH}/ecl_fmt ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.UNRST ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.DATA)


add_executable( ecl_rsthead ecl_rsthead.c )
target_link_libraries( ecl_rsthead ecl test_util )
add_test( ecl_rsthead ${EXECUTABLE_OUTPUT_PATH}/ecl_rsthead ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.UNRST 
                                                            ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/DualPoro/DUALPORO.X0005 ) 

add_executable( ecl_rft ecl_rft.c )
target_link_libraries( ecl_rft ecl test_util )
add_test( ecl_rft_rft ${EXECUTABLE_OUTPUT_PATH}/ecl_rft ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.RFT RFT)
add_test( ecl_rft_rft_rw ${EXECUTABLE_OUTPUT_PATH}/ecl_rft ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.RFT RFT_RW)
add_test( ecl_rft_plt ${EXECUTABLE_OUTPUT_PATH}/ecl_rft ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/RFT/TEST1_1A.RFT PLT)
add_test( ecl_rft_plt ${EXECUTABLE_OUTPUT_PATH}/ecl_rft ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/RFT/RFT2.RFT MSW-PLT)

add_executable( ecl_rft_cell ecl_rft_cell.c )
target_link_libraries( ecl_rft_cell ecl test_util )
add_test( ecl_rft_cell ${EXECUTABLE_OUTPUT_PATH}/ecl_rft_cell )

add_executable( ecl_grid_copy ecl_grid_copy.c )
target_link_libraries( ecl_grid_copy ecl test_util )
add_test( ecl_grid_copy ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_copy )

add_executable( ecl_grid_copy_statoil ecl_grid_copy_statoil.c )
target_link_libraries( ecl_grid_copy_statoil ecl test_util )

add_test( ecl_grid_copy_statoil1 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_copy_statoil ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Gurbat/ECLIPSE.EGRID )

add_test( ecl_grid_copy_statoil2 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_copy_statoil ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Mariner/MARINER.EGRID )

add_test( ecl_grid_copy_statoil3 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_copy_statoil ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/LGCcase/LGC_TESTCASE2.EGRID )

add_test( ecl_grid_copy_statoil4 ${EXECUTABLE_OUTPUT_PATH}/ecl_grid_copy_statoil ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/10kcase/TEST10K_FLT_LGR_NNC.EGRID )



add_executable( ecl_get_num_cpu ecl_get_num_cpu_test.c )
target_link_libraries( ecl_get_num_cpu ecl test_util )
add_test( ecl_get_num_cpu ${EXECUTABLE_OUTPUT_PATH}/ecl_get_num_cpu 
          ${PROJECT_SOURCE_DIR}/libecl/tests/data/num_cpu1 
          ${PROJECT_SOURCE_DIR}/libecl/tests/data/num_cpu2 
          ${PROJECT_SOURCE_DIR}/libecl/tests/data/num_cpu3
          ${PROJECT_SOURCE_DIR}/libecl/tests/data/num_cpu4 )


add_executable( ecl_fault_block_layer_statoil ecl_fault_block_layer_statoil.c )
target_link_libraries( ecl_fault_block_layer_statoil ecl test_util )
add_test( ecl_fault_block_layer_statoil ${EXECUTABLE_OUTPUT_PATH}/ecl_fault_block_layer_statoil 
          ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Mariner/MARINER.EGRID
          ${PROJECT_SOURCE_DIR}/test-data/Statoil/ECLIPSE/Mariner/faultblock.grdecl)


add_executable( ecl_fault_block_layer ecl_fault_block_layer.c )
target_link_libraries( ecl_fault_block_layer ecl test_util )
add_test( ecl_fault_block_layer ${EXECUTABLE_OUTPUT_PATH}/ecl_fault_block_layer ) 

set_property( TEST ecl_fault_block_layer_statoil      PROPERTY LABELS StatoilData )
set_property( TEST ecl_fmt              PROPERTY LABELS StatoilData )
set_property( TEST ecl_coarse_test      PROPERTY LABELS StatoilData )
set_property( TEST ecl_restart_test     PROPERTY LABELS StatoilData )
set_property( TEST ecl_lgr_test1        PROPERTY LABELS StatoilData )
set_property( TEST ecl_lgr_test2        PROPERTY LABELS StatoilData )
set_property( TEST ecl_lgr_test3        PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_lgr_name    PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_simple      PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_export      PROPERTY LABELS StatoilData )
set_property( TEST ecl_dualp            PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_test         PROPERTY LABELS StatoilData )

set_property( TEST ecl_sum_report_step_equal1 PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_report_step_equal2 PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_report_step_equal3 PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_report_step_equal4 PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_report_step_equal5 PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_report_step_equal6 PROPERTY LABELS StatoilData )

set_property( TEST ecl_sum_report_step_compatible1 PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_report_step_compatible2 PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_report_step_compatible3 PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_report_step_compatible4 PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_report_step_compatible5 PROPERTY LABELS StatoilData )
set_property( TEST ecl_sum_report_step_compatible6 PROPERTY LABELS StatoilData )



set_property( TEST ecl_fortio PROPERTY LABELS StatoilData)
set_property( TEST ecl_grid_dims1 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_dims2 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_dims3 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_dims4 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_dims5 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_test1 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_test2 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_test3 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_test4 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_test5 PROPERTY LABELS StatoilData )

set_property( TEST ecl_grid_fwrite PROPERTY LABELS StatoilData)
set_property( TEST ecl_file PROPERTY LABELS StatoilData)
set_property( TEST ecl_rsthead PROPERTY LABELS StatoilData)
set_property( TEST ecl_region PROPERTY LABELS StatoilData)
set_property( TEST ecl_region2region PROPERTY LABELS StatoilData)
set_property( TEST ecl_grid_case PROPERTY LABELS StatoilData)
set_property( TEST ecl_rft_rft PROPERTY LABELS StatoilData)
set_property( TEST ecl_rft_plt PROPERTY LABELS StatoilData)
set_property( TEST ecl_rft_rft_rw PROPERTY LABELS StatoilData)
set_property( TEST ecl_sum_case_exists PROPERTY LABELS StatoilData)
set_property( TEST ecl_grid_volume1 PROPERTY LABELS StatoilData)
set_property( TEST ecl_grid_volume2 PROPERTY LABELS StatoilData)
#set_property( TEST ecl_grid_volume3 PROPERTY LABELS StatoilData)
set_property( TEST ecl_grid_cell_contains2 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_cell_contains3 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_cell_contains4 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_cell_volume2 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_cell_volume3 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_export1 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_export2 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_export3 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_export4 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_export5 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_export6 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_export7 PROPERTY LABELS StatoilData )
set_property( TEST ecl_nnc_export_get_tran PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_cell_contains2 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_copy_statoil1 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_copy_statoil2 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_copy_statoil3 PROPERTY LABELS StatoilData )
set_property( TEST ecl_grid_copy_statoil4 PROPERTY LABELS StatoilData )
set_property( TEST ecl_layer_statoil PROPERTY LABELS StatoilData )
