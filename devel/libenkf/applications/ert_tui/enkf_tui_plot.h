/*
   Copyright (C) 2011  Statoil ASA, Norway. 
    
   The file 'enkf_tui_plot.h' is part of ERT - Ensemble based Reservoir Tool. 
    
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

#ifndef __ENKF_TUI_PLOT_H__
#define __ENKF_TUI_PLOT_H__


#include <ert/util/matrix.h>

#include <ert/enkf/pca_plot_data.h>
#include <ert/enkf/enkf_main.h>

void    enkf_tui_plot_simple_menu(void * );
void    enkf_tui_plot_menu(void * );
void    enkf_tui_plot_PC( enkf_main_type * enkf_main , const char * plot_name , const pca_plot_data_type * plot_data);
void    enkf_tui_plot_reports(void *);
void    enkf_tui_plot_all_summary__( enkf_main_type * enkf_main , int iens1 , int iens2 , int step1 , int step2 , bool prediction_mode);

void    enkf_tui_plot_ensemble__(enkf_main_type * enkf_main , 
                                 const enkf_config_node_type * config_node , 
                                 const char * user_key  ,
                                 const char * key_index ,
                                 int step1 , int step2  , 
                                 bool prediction_mode   ,
                                 int iens1 , int iens2  , 
                                 state_enum plot_state);
#endif
