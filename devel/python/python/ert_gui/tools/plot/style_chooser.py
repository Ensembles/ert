from PyQt4.QtGui import QWidget, QHBoxLayout, QComboBox, QDoubleSpinBox, QLabel, QHBoxLayout

from ert_gui.plottery import PlotStyle


STYLE_OFF = ("Off", None)
STYLE_AREA = ("Area", "#")
STYLE_SOLID = ("Solid", "-")
STYLE_DASHED = ("Dashed", "--")
STYLE_DOTTED = ("Dotted", ":")
STYLE_DASH_DOTTED = ("Dash Dotted", "-.")

STYLES = [STYLE_OFF, STYLE_AREA, STYLE_SOLID, STYLE_DASHED, STYLE_DOTTED, STYLE_DASH_DOTTED]
STYLES_LINE_ONLY = [STYLE_OFF, STYLE_SOLID, STYLE_DASHED, STYLE_DOTTED, STYLE_DASH_DOTTED]

MARKER_OFF = ("Off", None)
MARKER_X = ("X", "x")
MARKER_CIRCLE = ("Circle", "o")
MARKER_POINT = ("Point", ".")
MARKER_STAR = ("Star", "*")
MARKER_DIAMOND = ("Diamond", "D")

MARKERS = [MARKER_OFF, MARKER_X, MARKER_CIRCLE, MARKER_POINT, MARKER_STAR, MARKER_DIAMOND]


class StyleChooser(QWidget):

    def __init__(self, area_supported=False):
        QWidget.__init__(self)
        self._style = PlotStyle("StyleChooser Internal Style")
        self._styles = STYLES if area_supported else STYLES_LINE_ONLY

        self.setMinimumWidth(140)
        self.setMaximumHeight(25)

        layout = QHBoxLayout()
        layout.setMargin(0)
        layout.setSpacing(2)

        self.line_chooser = QComboBox()
        self.line_chooser.setToolTip("Select line style.")
        for style in self._styles:
            self.line_chooser.addItem(*style)

        self.marker_chooser = QComboBox()
        self.marker_chooser.setToolTip("Select marker style.")
        for marker in MARKERS:
            self.marker_chooser.addItem(*marker)

        self.thickness_spinner = QDoubleSpinBox()
        self.thickness_spinner.setToolTip("Line thickness")
        self.thickness_spinner.setMinimum(0.1)
        self.thickness_spinner.setDecimals(1)
        self.thickness_spinner.setSingleStep(0.1)

        layout.addWidget(self.line_chooser)
        layout.addWidget(self.marker_chooser)
        layout.addWidget(self.thickness_spinner)

        self.setLayout(layout)

        self.line_chooser.currentIndexChanged.connect(self._updateStyle)
        self.marker_chooser.currentIndexChanged.connect(self._updateStyle)
        self.thickness_spinner.valueChanged.connect(self._updateStyle)

        self._updateLineStyleAndMarker(self._style.line_style, self._style.marker, self._style.width)
        self._layout = layout

    def getItemSizes(self):
        line_style_combo_width = self._layout.itemAt(0).sizeHint().width()
        marker_combo_width = self._layout.itemAt(1).sizeHint().width()
        thickness_spinner_width = self._layout.itemAt(2).sizeHint().width()
        return line_style_combo_width, marker_combo_width, thickness_spinner_width

    def _findLineStyleIndex(self, line_style):
        for index, style in enumerate(self._styles):
            if style[1] == line_style:
                return index
            elif style[1] is None and line_style == "":
                return index
        return -1

    def _findMarkerStyleIndex(self, marker):
        for index, style in enumerate(MARKERS):
            if style[1] == marker:
                return index
            elif style[1] is None and marker == "":
                return index
        return -1

    def _updateLineStyleAndMarker(self, line_style, marker, thickness):
        self.line_chooser.setCurrentIndex(self._findLineStyleIndex(line_style))
        self.marker_chooser.setCurrentIndex(self._findMarkerStyleIndex(marker))
        self.thickness_spinner.setValue(thickness)

    def _updateStyle(self):
        self.marker_chooser.setEnabled(self.line_chooser.currentText() != "Area")

        line_style = self.line_chooser.itemData(self.line_chooser.currentIndex())
        marker_style = self.marker_chooser.itemData(self.marker_chooser.currentIndex())
        thickness = float(self.thickness_spinner.value())

        self._style.line_style = str(line_style.toString())
        self._style.marker = str(marker_style.toString())
        self._style.width = thickness

    def setStyle(self, style):
        """ @type style: PlotStyle """
        self._style.copyStyleFrom(style)
        self._updateLineStyleAndMarker(style.line_style, style.marker, style.width)

    def getStyle(self):
        """ @rtype: PlotStyle """
        style = PlotStyle("Generated Style from StyleChooser")
        style.copyStyleFrom(self._style)
        return style

    def createLabelLayout(self, layout=None):
        if layout is None:
            layout = QHBoxLayout()

        titles = ["Line Style", "Marker Style", "Width"]
        sizes = self.getItemSizes()
        for title, size in zip(titles, sizes):
            label = QLabel(title)
            label.setFixedWidth(size)
            layout.addWidget(label)

        return layout

