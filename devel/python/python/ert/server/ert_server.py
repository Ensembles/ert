#  Copyright (C) 2014  Statoil ASA, Norway. 
#   
#  The file 'ert_server.py' is part of ERT - Ensemble based Reservoir Tool. 
#   
#  ERT is free software: you can redistribute it and/or modify 
#  it under the terms of the GNU General Public License as published by 
#  the Free Software Foundation, either version 3 of the License, or 
#  (at your option) any later version. 
#   
#  ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or 
#  FITNESS FOR A PARTICULAR PURPOSE.   
#   
#  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
#  for more details. 

import sys
import threading
import json
import os

from ert.enkf import EnKFMain,RunArg
from ert.enkf.enums import EnkfRunType , EnkfStateType
from ert.enkf import NodeId

from .run_context import RunContext

class ErtCmdError(Exception):
    pass



class ErtServer(object):
    site_config = None

    def __init__(self , config_file = None):
        self.ert_handle = None
        if config_file:
            if os.path.exists(config_file):
                self.open( config_file )
            else:
                raise IOError("The config file:%s does not exist" % config_file)
        self.initCmdTable()
        self.run_context = None



    def initCmdTable(self):
        self.cmd_table = {"STATUS" : self.handleSTATUS ,
                          "INIT_SIMULATIONS" : self.handleINIT_SIMULATIONS ,
                          "ADD_SIMULATION" : self.handleADD_SIMULATION ,
                          "SET_VARIABLE" : self.handleSET_VARIABLE }


    def open(self , config_file):
        self.config_file = config_file
        self.ert_handle = EnKFMain( config_file , ErtServer.site_config )
        


    def close(self):
        # More cleanup first ...
        self.ert_handle = None


    def isConnected(self):
        if self.ert_handle:
            return True
        else:
            return False


    def __del__(self):
        if self.isConnected():
            self.close()



    def evalCmd(self , cmd_expr):
        cmd = cmd_expr[0]
        func = self.cmd_table.get(cmd)

        if func:
            return func(cmd_expr[1:])
        else:
            raise ErtCmdError("The command:%s was not recognized" % cmd)


    def handleSTATUS(self , args):
        if self.isConnected():
            if self.run_context is None:
                return ["READY"]
            else:
                return ["RUNNING" , self.run_context.getNumRunning() , self.run_context.getNumComplete()]
        else:
            return ["CLOSED"]


    def handleINIT_SIMULATIONS(self , args):
        if len(args) == 2:
            if self.run_context is None:
                run_size = args[0]
                init_case = args[1]
                self.run_context = RunContext(self.ert_handle , run_size , init_case)
                return self.handleSTATUS([])
            else:
                raise ErtCmdError("The ert server has already started simulations")
        else:
            raise ErtCmdError("The INIT_SIMULATIONS command expects two arguments: [ensemble_size , init_case]")



    def handleSET_VARIABLE(self , args):
        iens = args[0]
        kw = str(args[1])
        ensembleConfig = self.ert_handle.ensembleConfig()
        if ensembleConfig.hasKey(kw):
            state = self.ert_handle[iens]
            node = state[kw]
            gen_kw = node.asGenKw()
            print "Arguments: %s" % args[2:]
            gen_kw.setValues(args[2:])
            
            fs = self.ert_handle.getEnkfFsManager().getCurrentFileSystem()
            node_id = NodeId(0 , iens , EnkfStateType.ANALYZED )
            node.save( fs , node_id )
        else:
            raise ErtCmdError("The keyword:%s is not recognized" % kw)
            


    def handleADD_SIMULATION(self , args):
        iens = args[0]
        run_arg = RunArg(EnkfRunType.ENSEMBLE_EXPERIMENT , iens , "simulations/run%s" % iens)
        self.ert_handle.submitSimulation( run_arg )
