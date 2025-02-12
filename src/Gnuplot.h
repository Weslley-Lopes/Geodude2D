#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
#include <memory>
#include "OutputGraphic.h"

class Gnuplot
{
public:
    Gnuplot(const std::string &outputFile = "");
    ~Gnuplot();

    void setRange(const std::string &axis, double min, double max);
    void setTitle(const std::string &title);
    void setXLabel(const std::string &label);
    void setYLabel(const std::string &label);
    void setXTics(double step);
    void setYTics(double step);
    void enableGrid(const std::string &axis);
    void disableGrid(const std::string &axis);
    void plotSeparate(std::vector<OutputGraphic *> graphics);
    void plotSeparate(OutputGraphic *graphics);
    void plotTogether(std::vector<OutputGraphic *> graphics);
    void plotTogether(std::vector<OutputGraphic *> graphics, std::string style, const int &frequency = 1);
    void getTitleAndFileName(std::vector<OutputGraphic *> graphics, std::string &title, std::string &fileName, bool test[3]);
    std::string getLegend(OutputGraphic *graph, bool test[3]);
    void addPlot(OutputGraphic *graphic, const std::string &title, const std::string &style, const std::string &color);
    void plot();
    void view() const;

    static void killAllGnuplot();

private:
    struct Plot
    {
        Plot(const std::string &dataFile, const std::string &title, const std::string &style, const std::string &pointStyle, const std::string &color);
        std::string dataFile;
        std::string title;
        std::string style;
        std::string pointStyle;
        std::string color;
    };

    std::vector<Plot> plots;
    std::string outputFile;
    std::string viewData;
    std::vector<std::string> colors_;
    std::vector<std::string> pointStyles_;
    std::string frequency_;
    FILE *gp;
};