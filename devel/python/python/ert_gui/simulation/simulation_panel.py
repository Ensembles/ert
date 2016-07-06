from PyQt4.QtCore import Qt, QSize
from PyQt4.QtGui import QWidget, QVBoxLayout, QHBoxLayout, QLabel, QStackedWidget, QFrame, QToolButton, QMessageBox, QComboBox

from ert_gui import ERT
from ert_gui.ertwidgets import addHelpToWidget
from ert_gui.ertwidgets.models.ertmodel import getCurrentCaseName
from ert_gui.pages.run_dialog import RunDialog
from ert_gui.simulation import EnsembleExperimentPanel, EnsembleSmootherPanel
from ert_gui.simulation import IteratedEnsembleSmootherPanel, MultipleDataAssimilationPanel, SimulationConfigPanel
from ert_gui.widgets import util


class SimulationPanel(QWidget):

    def __init__(self):
        QWidget.__init__(self)

        layout = QVBoxLayout()

        self._simulation_mode_combo = QComboBox()
        addHelpToWidget(self._simulation_mode_combo, "run/simulation_mode")

        self._simulation_mode_combo.currentIndexChanged.connect(self.toggleSimulationMode)

        simulation_mode_layout = QHBoxLayout()
        simulation_mode_layout.addSpacing(10)
        simulation_mode_layout.addWidget(QLabel("Simulation mode:"), 0, Qt.AlignVCenter)
        simulation_mode_layout.addWidget(self._simulation_mode_combo, 0, Qt.AlignVCenter)

        simulation_mode_layout.addSpacing(20)

        self.run_button = QToolButton()
        self.run_button.setIconSize(QSize(32, 32))
        self.run_button.setText("Start Simulation")
        self.run_button.setIcon(util.resourceIcon("ide/gear_in_play"))
        self.run_button.clicked.connect(self.runSimulation)
        self.run_button.setToolButtonStyle(Qt.ToolButtonTextBesideIcon)
        addHelpToWidget(self.run_button, "run/start_simulation")

        simulation_mode_layout.addWidget(self.run_button)
        simulation_mode_layout.addStretch(1)

        layout.addSpacing(5)
        layout.addLayout(simulation_mode_layout)
        layout.addSpacing(10)

        self.simulation_stack = QStackedWidget()
        self.simulation_stack.setLineWidth(1)
        self.simulation_stack.setFrameStyle(QFrame.StyledPanel)

        layout.addWidget(self.simulation_stack)

        self.simulation_widgets = {}

        self.addSimulationConfigPanel(EnsembleExperimentPanel())
        self.addSimulationConfigPanel(EnsembleSmootherPanel())
        self.addSimulationConfigPanel(IteratedEnsembleSmootherPanel())
        self.addSimulationConfigPanel(MultipleDataAssimilationPanel())

        self.setLayout(layout)


    def addSimulationConfigPanel(self, panel):
        assert isinstance(panel, SimulationConfigPanel)

        panel.toggleAdvancedOptions(False)
        self.simulation_stack.addWidget(panel)

        simulation_model = panel.getSimulationModel()

        self.simulation_widgets[simulation_model] = panel
        self._simulation_mode_combo.addItem(str(simulation_model), simulation_model)
        panel.simulationConfigurationChanged.connect(self.validationStatusChanged)


    def getActions(self):
        return []


    def toggleAdvancedMode(self, show_advanced):
        for panel in self.simulation_widgets.values():
            panel.toggleAdvancedOptions(show_advanced)


    def getCurrentSimulationMode(self):
        data = self._simulation_mode_combo.itemData(self._simulation_mode_combo.currentIndex(), Qt.UserRole)
        return data.toPyObject()


    def runSimulation(self):
        case_name = getCurrentCaseName()
        message = "Are you sure you want to use case '%s' for initialization of the initial ensemble when running the simulations?" % case_name
        start_simulations = QMessageBox.question(self, "Start simulations?", message, QMessageBox.Yes | QMessageBox.No )

        if start_simulations == QMessageBox.Yes:
            run_model = self.getCurrentSimulationMode()

            dialog = RunDialog(run_model, self)
            dialog.startSimulation()
            dialog.exec_()

            ERT.emitErtChange() # simulations may have added new cases.


    def toggleSimulationMode(self):
        widget = self.simulation_widgets[self.getCurrentSimulationMode()]
        self.simulation_stack.setCurrentWidget(widget)
        self.validationStatusChanged()


    def validationStatusChanged(self):
        widget = self.simulation_widgets[self.getCurrentSimulationMode()]
        self.run_button.setEnabled(widget.isConfigurationValid())
