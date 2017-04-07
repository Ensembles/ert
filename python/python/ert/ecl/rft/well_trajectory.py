#  Copyright (C) 2015  Statoil ASA, Norway. 
#   
#  The file 'well_trajectory.py' is part of ERT - Ensemble based Reservoir Tool. 
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
import os
from collections import namedtuple
from cwrap import Stream

TrajectoryPoint = namedtuple("TrajectoryPoint", "utm_x utm_y measured_depth true_vertical_depth zone")

class WellTrajectory(object):

    def __init__(self , filename):
        if not os.path.isfile(filename):
            raise IOError('No such file "%s".' % filename)

        self.points = []
        with open(filename) as fileH:
            for line in fileH:
                self._append(line)

    def _append(self, line):
        """Appends a content of a TrajectoryPoint line to self.points."""
        line = line.split('--')[0].strip()
        if not line:
            return
        point = line.split()
        if len(point) not in (4, 5):
            raise UserWarning("Trajectory data file not on correct format: \"utm_x utm_y md tvd [zone]\" - zone is optional")
        try:
            utm_x = float(point[0])
            utm_y = float(point[1])
            md = float(point[2])
            tvd = float(point[3])
            if len(point) > 4:
                zone = point[4]
            else:
                zone = None
            self.points.append(TrajectoryPoint(utm_x , utm_y , md , tvd , zone))
        except ValueError:
            raise UserWarning("Warning: Failed to extract data from line %s\n" % line)

    def __len__(self):
        return len(self.points)

        
    def __getitem__(self , index):
        if index < 0:
            index += len(self)

        return self.points[index]

    def __repr__(self):
        return repr(self.points)

    def __str__(self):
        return str(self.points)
