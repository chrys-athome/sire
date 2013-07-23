
from matplotlib import pyplot

def plotAll(histograms):
    figure = pyplot.figure()
    axes = figure.add_subplot(111)

    for histogram in histograms:
        x = []
        y = []
        for value in histogram.values():
            x.append(value.middle())
            y.append(value.value())

            axes.plot(x, y)

    pyplot.show()

def plotPoints(points):
    figure = pyplot.figure()
    axes = figure.add_subplot(111)

    x = []
    y = []
    ymin = []
    ymax = []

    for point in points:
        x.append(point.x())
        y.append(point.y())
        ymin.append(point.y() - point.yError())
        ymax.append(point.y() + point.yError())

    axes.plot(x, y)
    axes.plot(x, ymin)
    axes.plot(x, ymax)

    pyplot.show()

