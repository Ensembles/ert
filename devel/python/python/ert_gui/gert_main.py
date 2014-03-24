#  Copyright (C) 2011  Statoil ASA, Norway. 
#   
#  The file 'gert_main.py' is part of ERT - Ensemble based Reservoir Tool. 
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

#--------------------------------------------------------------------------------
# This file is the main script of the ert with graphical UI, e.g. gert or
# ert_gui. To run successfully the ert GUI requires a quite well prepared
# environment. This includes the following:
#
#  1. A Python interpreter with the the ctypes library available.
#
#  2. The program must be able to locate all the required shared libraries with
#     no fuss. This includes:
#
#       o The ert libraries libecl.so, libenkf.so, libutil.so, librms.so,
#         libsched.so, libconfig.so
#
#       o The libraries used by the ert libraries, this includes liblapack.so,
#         libz.so, libblas.so, libpthread.so and in some cases the libg2c.so
#         library.
#
#       o The lsf libraries libbat, liblsf and also libnsl. In the current
#         implementation the dependance on the lsf libraries is hard, this should
#         be relaxed so that the lsf libraries are not linked before an attempt to
#         actually use lsf is made.
#         When an attempt is actually made to use LSF the additional environment
#         variables LSF_LIBDIR, XLSF_UIDDIR, LSF_SERVERDIR, LSF_ENVDIR and
#         LSF_BINDIR must also be set. That is an LSF requirement and not
#         related to ert as such. These variables can naturally be set from the
#         site config file.
#  
#
#  3. The program must be able to locate all the necessary Python modules, in
#     short this means that the directory containing the ert/ and ert_gui/
#     directories must be on Python path, i.e. the import statements
#
#        import ert
#        import ert_gui
#    
#     should just work.
#
#  4. The environment variable GERT_SHARE_PATH should be set to point to the
#     /share directory of the current gert installation. The /share directory
#     contains html help files and images/icons.
#
#  5. The environment variable ERT_SITE_CONFIG must be set to point to the site
#     wide configuration file.
#
#
# Now the important point is that this python script WILL NOT PERFORM ANY
# SPECIAL HOOPS TO TRY TO LOCATE THE REQUIRED FILES, i.e. the environment must
# be fully prepared prior to invoking this script. This will typically involve:
#
#  1. Update the LD_LIBRARY_PATH variable to contain directories with all the
#     required shared libraries.
#
#  2. Update the PYTHONPATH variable to contain the directory containg ert/ and
#     ert_gui/ directories.
#
#  3. Set the environment variabel GERT_SHARE_PATH to point to the directory
#     containg the /share files for the current gert installation.
#
#  4. Set the environment variable ERT_SITE_CONFIG to point to the location of
#     the site configuration file.
#
# An example shell script achieving this could look like:
#
#-------------------- <Example shell script> --------------------
#  #!/bin/bash
#
#  # The LSF libraries are installed in directory /site/LSF/7.0/linux/lib, this
#  # directory must be included in the LD_LIBRARY_PATH variable. Furthermore we
#  # assume that the ERT libraries like libecl.so and libenkf.so are located in
#  # /opt/ert/lib, then LD_LIBRARY_PATH will be updated as:
#  
#  export LD_LIBRARY_PATH=/site/LSF/7.0/linux/lib:/opt/ert/lib:$LD_LIBRARY_PATH
#
#  # The python modules ert and ert_gui are located in /opt/ert/python, so we
#  # update PYTHONPATH as:
#
#  export PYTHONPATH=/opt/ert/python:$PYTHONPATH
#
#  # The shared gert files are installed in /opt/ert/share; this directory can
#  # in principle be shared among gert versions built for different operating
#  # system versions:
#
#  export ERT_SHARE_PATH=/opt/ert/share
#
#  # The ERT site configuration file is assumed to be in
#  # /opt/ert/etc/site-config, i.e. we set the variable ERT_SITE_CONFIG as:
#
#  export ERT_SITE_CONFIG=/opt/ert/etc/site-config
#
#  # Now the environment should be fully initialized, and we are ready to invoke
#  # the gert_main.py script, i.e. this file:
#
#  exec python /opt/ert/python/ert_gui/gert_main.py $@
#
#-------------------- </Example shell script> --------------------

import sys
import os
from PyQt4.QtCore import Qt
from PyQt4.QtGui import QApplication, QSplashScreen
from ert.enkf import EnKFMain
from ert_gui.main_window import GertMainWindow
from ert_gui.models import ErtConnector
from ert_gui.pages.summary_panel import SummaryPanel
from ert_gui.simulation.simulation_panel import SimulationPanel
from ert_gui.tools import HelpCenter

from ert_gui.tools.help import HelpTool
from ert_gui.tools.ide import IdeTool
from ert_gui.tools.manage_cases import ManageCasesTool
from ert_gui.tools.plot import PlotTool
from ert_gui.tools.export import ExportTool
from ert_gui.tools.workflows import WorkflowsTool
from ert_gui.widgets import util

import ert_gui.widgets.util

ert_gui.widgets.util.img_prefix = os.getenv("ERT_SHARE_PATH") + "/gui/img/"

from ert_gui.newconfig import NewConfigurationDialog

from ert_gui.widgets.util import resourceImage


class Ert(object):
    def __init__(self, enkf_main):
        super(Ert, self).__init__()

        assert isinstance(enkf_main, EnKFMain)
        self.__ert = enkf_main

    def reloadERT(self):
        python_executable = sys.executable
        ert_gui_main = sys.argv[0]
        config_file = self.__ert.getUserConfigFile()

        self.__ert.free()
        os.execl(python_executable, python_executable, ert_gui_main, config_file)

    def ert(self):
        return self.__ert


def main(argv):
    app = QApplication(argv) #Early so that QT is initialized before other imports
    app.setWindowIcon(util.resourceIcon("application/window_icon_cutout"))

    splash = QSplashScreen(resourceImage("newsplash"), Qt.WindowStaysOnTopHint)
    splash.show()
    splash.showMessage("Starting up...", Qt.AlignLeft, Qt.white)
    app.processEvents()

    help_center = HelpCenter("ERT")
    help_center.setHelpLinkPrefix(os.getenv("ERT_SHARE_PATH") + "/gui/help/")
    help_center.setHelpMessageLink("welcome_to_ert")

    splash.showMessage("Bootstrapping...", Qt.AlignLeft, Qt.white)
    app.processEvents()

    strict = True
    site_config = os.getenv("ERT_SITE_CONFIG")
    if len(argv) == 1:
        print("-----------------------------------------------------------------")
        print("-- You must supply the name of configuration file as the first --")
        print("-- commandline argument:                                       --")
        print("--                                                             --")
        print("-- bash%  gert <config_file>                                   --")
        print("--                                                             --")
        print("-- If the configuration file does not exist, gert will create  --")
        print("-- create a new configuration file.                            --")
        print("-----------------------------------------------------------------")
    else:
        enkf_config = argv[1]
        if not os.path.exists(enkf_config):
            print("Trying to start new config")
            new_configuration_dialog = NewConfigurationDialog(enkf_config)
            success = new_configuration_dialog.exec_()
            if not success:
                print("Can not run without a configuration file.")
                sys.exit(1)
            else:
                enkf_config = new_configuration_dialog.getConfigurationPath()
                first_case_name = new_configuration_dialog.getCaseName()
                dbase_type = new_configuration_dialog.getDBaseType()
                num_realizations = new_configuration_dialog.getNumberOfRealizations()
                storage_path = new_configuration_dialog.getStoragePath()

                EnKFMain.createNewConfig(enkf_config, storage_path, first_case_name, dbase_type, num_realizations)
                strict = False

        ert = Ert(EnKFMain(enkf_config, site_config=site_config, strict=strict))
        ErtConnector.setErt(ert.ert())


        splash.showMessage("Creating GUI...", Qt.AlignLeft, Qt.white)
        app.processEvents()


        window = GertMainWindow()
        window.setWidget(SimulationPanel())

        help_tool = HelpTool("ERT", window)

        window.addDock("Configuration Summary", SummaryPanel(), area=Qt.BottomDockWidgetArea)
        window.addTool(IdeTool(os.path.basename(enkf_config), ert.reloadERT, help_tool))
        window.addTool(PlotTool())
        window.addTool(ExportTool())
        window.addTool(WorkflowsTool(ert.reloadERT))
        window.addTool(ManageCasesTool())
        window.addTool(help_tool)


        splash.showMessage("Communicating with ERT...", Qt.AlignLeft, Qt.white)
        app.processEvents()

        window.show()
        splash.finish(window)

        finished_code = app.exec_()

        ert.ert().free()

        sys.exit(finished_code)


if __name__ == "__main__":
    main(sys.argv)







