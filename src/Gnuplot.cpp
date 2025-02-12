#include "Gnuplot.h"

Gnuplot::Gnuplot(const std::string &outputFile)
    : outputFile(outputFile),
      colors_{"blue", "red", "black", "purple", "cyan", "gray", "magenta", "brown"},
      pointStyles_{" pt 5", // circle
                   " pt 4", // square
                   " pt 6", " pt 10", " pt 8", " pt 7", " pt 3", " pt 9"},
      frequency_("")
{
    gp = popen("gnuplot", "w");
    if (!gp)
    {
        throw std::runtime_error("Failed to open pipe to gnuplot.");
    }
}

Gnuplot::~Gnuplot()
{
    if (gp)
    {
        fprintf(gp, "exit\n");
        pclose(gp);
    }
}

void Gnuplot::setRange(const std::string &axis, double min, double max)
{
    if (axis == "y")
    {
        fprintf(gp, "set autoscale y\n");
        viewData += "set autoscale y\n";
    }
    else
    {
        fprintf(gp, "set %srange [%f:%f]\n", axis.c_str(), min, max);
        std::ostringstream ss;
        ss << "set " << axis << "range [" << min << ":" << max << "]\n";
        viewData += ss.str();
    }
}

void Gnuplot::setTitle(const std::string &title)
{
    fprintf(gp, "set title \"%s\"\n", title.c_str());
    viewData += "set title \"" + title + "\"\n";
}

void Gnuplot::setXLabel(const std::string &label)
{
    fprintf(gp, "set xlabel \"%s\"\n", label.c_str());
    viewData += "set xlabel \"" + label + "\"\n";
}

void Gnuplot::setYLabel(const std::string &label)
{
    fprintf(gp, "set ylabel \"%s\"\n", label.c_str());
    viewData += "set ylabel \"" + label + "\"\n";
}

void Gnuplot::setXTics(double step)
{
    fprintf(gp, "set xtics %f\n", step);
    viewData += "set xtics " + std::to_string(step) + "\n";
}

void Gnuplot::setYTics(double step)
{
    if (step == 0.0)
    {
        fprintf(gp, "set ytics auto");
        viewData += "set ytics auto\n";
    }
    else
    {
        fprintf(gp, "set ytics %f\n", step);
        viewData += "set ytics " + std::to_string(step) + "\n";
    }
}

void Gnuplot::addPlot(OutputGraphic *graphic, const std::string &title, const std::string &style, const std::string &color)
{
    std::string aux1, aux2;
    switch (graphic->getVariable())
    {
    case POSITION:
        aux1 = "Position";
        break;
    case VELOCITY:
        aux1 = "Velocity";
        break;
    case ACCELERATION:
        aux1 = "Acceleration";
        break;
    case CONTACT_FORCE:
        aux1 = "Contact Force";
        break;
    default:
        break;
    }
    switch (graphic->getConstrainedDOF())
    {
    case X:
        aux1 = "x";
        break;
    case Y:
        aux1 = "y";
        break;
    case Z:
        aux1 = "z";
        break;
    default:
        break;
    }
    std::string aux = "";
    plots.emplace_back("plotData/" + graphic->getFileName() + ".dat", title, style, aux, color);
}

void Gnuplot::plotSeparate(std::vector<OutputGraphic *> graphics)
{
    for (auto &g : graphics)
    {
        std::string aux1, aux2;
        switch (g->getVariable())
        {
        case POSITION:
            aux1 = "Position";
            break;
        case VELOCITY:
            aux1 = "Velocity";
            break;
        case ACCELERATION:
            aux1 = "Acceleration";
            break;
        case CONTACT_FORCE:
            aux1 = "Contact Force";
            break;
        case ROTATION:
            aux1 = "Rotation";
            break;
        case ANGULAR_VELOCITY:
            aux1 = "Angular Velocity";
            break;
        case ANGULAR_ACCELERATION:
            aux1 = "Angular Acceleration";
            break;
        default:
            break;
        }
        switch (g->getConstrainedDOF())
        {
        case X:
            aux2 = "x";
            break;
        case Y:
            aux2 = "y";
            break;
        case Z:
            aux2 = "z";
            break;
        default:
            break;
        }
        plots.clear();
        viewData = "";
        std::string title = aux1 + " in " + aux2;
        std::string style = "lines";
        plots.emplace_back("plotData/" + g->getFileName() + ".dat", "", style, "", colors_[0]);
        setTitle(title);
        setXLabel("Time");
        setYLabel(aux1);
        // setRange("x", g->getXMin(), g->getXMax());
        // setRange("y", 0.0, 0.0);
        // setXTics((g->getXMax() - g->getXMin()) / 5.);
        // setYTics(0.0);

        if (!aux1.empty())
            aux1[0] = std::tolower(aux1[0]); // Converte apenas o primeiro caractere para minúscula
        outputFile = "plotGraph/" + g->getFileName() + ".png";
        plot();
    }
}

void Gnuplot::plotSeparate(OutputGraphic *g)
{
    std::string aux1, aux2;
    switch (g->getVariable())
    {
    case POSITION:
        aux1 = "Position";
        break;
    case VELOCITY:
        aux1 = "Velocity";
        break;
    case ACCELERATION:
        aux1 = "Acceleration";
        break;
    case CONTACT_FORCE:
        aux1 = "Contact Force";
        break;
    default:
        break;
    }
    switch (g->getConstrainedDOF())
    {
    case X:
        aux2 = "x";
        break;
    case Y:
        aux2 = "y";
        break;
    case Z:
        aux2 = "z";
        break;
    default:
        break;
    }
    plots.clear();
    viewData = "";
    std::string title = aux1 + " in " + aux2;
    std::string style = "lines";
    plots.emplace_back("plotData/" + g->getFileName() + ".dat", "", style, "", colors_[0]);
    setTitle(title);
    setXLabel("Time");
    setYLabel(aux1);
    setRange("x", g->getXMin(), g->getXMax());
    setRange("y", 0.0, 0.0);
    setXTics((g->getXMax() - g->getXMin()) / 5.);
    // setYTics(0.0);

    if (!aux1.empty())
        aux1[0] = std::tolower(aux1[0]); // Converte apenas o primeiro caractere para minúscula
    outputFile = "plotGraph/" + g->getFileName() + ".png";
    plot();
}

void Gnuplot::plotTogether(std::vector<OutputGraphic *> graphics)
{
    plots.clear();

    std::string title, fileName;
    bool test[3];
    getTitleAndFileName(graphics, title, fileName, test);
    setTitle(title);
    setXLabel("Time");
    setYLabel("Value");
    // setRange("x", g->getXMin(), g->getXMax());
    // setRange("y", 0.0, 0.0);
    // setXTics((g->getXMax() - g->getXMin()) / 5.);
    //  setYTics(0.0);
    int i = -1;
    int j = -1;
    std::string style = "lines";
    for (auto &g : graphics)
    {
        std::string legend = getLegend(g, test);
        std::string pointStyle = "";

        plots.emplace_back("plotData/" + g->getFileName() + ".dat", legend, style, pointStyle, colors_[++j]);
    }
    int particle = graphics[0]->getParticle();
    outputFile = "plotGraph/particle_" + std::to_string(particle) + "_" + fileName + ".png";
    plot();
}

void Gnuplot::plotTogether(std::vector<OutputGraphic *> graphics, std::string style, const int &frequency)
{
    plots.clear();

    std::string title, fileName;
    bool test[3];
    getTitleAndFileName(graphics, title, fileName, test);
    setTitle(title);
    setXLabel("Time");
    setYLabel("Value");
    // setRange("x", g->getXMin(), g->getXMax());
    // setRange("y", 0.0, 0.0);
    // setXTics((g->getXMax() - g->getXMin()) / 5.);
    //  setYTics(0.0);
    int i = -1;
    int j = -1;
    frequency_ = std::to_string(frequency);
    for (auto &g : graphics)
    {
        std::string legend = getLegend(g, test);
        std::string pointStyle = "";
        if (style == "linespoints" or style == "points")
            pointStyle = pointStyles_[++i];
        if (style == "linespoints")
        {
            plots.emplace_back("plotData/" + g->getFileName() + ".dat", legend, "points", pointStyle, colors_[++j]);
            plots.emplace_back("plotData/" + g->getFileName() + ".dat", "", "lines", "", colors_[j]);
        }
        else
            plots.emplace_back("plotData/" + g->getFileName() + ".dat", legend, style, pointStyle, colors_[++j]);
    }
    int particle = graphics[0]->getParticle();
    outputFile = "plotGraph/particle_" + std::to_string(particle) + "_" + fileName + ".png";
    plot();
}

void Gnuplot::getTitleAndFileName(std::vector<OutputGraphic *> graphics, std::string &title, std::string &fileName, bool test[3])
{
    bool node = false, var = false, dir = false;
    if (graphics[0]->getVariable() == STRAIN_ENERGY or graphics[0]->getVariable() == KINETIC_ENERGY or graphics[0]->getVariable() == EXTERNAL_ENERGY or graphics[0]->getVariable() == RIGIDITY_ENERGY or graphics[0]->getVariable() == CONTACT_ENERGY or graphics[0]->getVariable() == TOTAL_ENERGY)
    {
        var = true;
    }
    else
    {
        if (graphics[1]->getNodeIndex() == graphics[0]->getNodeIndex())
            node = true;
        if (graphics[1]->getVariable() == graphics[0]->getVariable())
            var = true;
        if (graphics[1]->getConstrainedDOF() == graphics[0]->getConstrainedDOF())
            dir = true;
    }
    test[0] = node;
    test[1] = var;
    test[2] = dir;

    if (node and var)
    {
        std::string aux;
        switch (graphics[0]->getVariable())
        {
        case POSITION:
            aux = "Positions";
            break;
        case VELOCITY:
            aux = "Velocities";
            break;
        case ACCELERATION:
            aux = "Accelerations";
            break;
        case CONTACT_FORCE:
            aux = "Contact Forces";
            break;
        default:
            break;
        }
        title = "Node " + std::to_string(graphics[0]->getNodeIndex()) + " " + aux;
        if (!aux.empty())
            aux[0] = std::tolower(aux[0]); // Converte apenas o primeiro caractere para minúscula
        fileName = "node_" + std::to_string(graphics[0]->getNodeIndex()) + "_" + aux;
    }
    else if (node and dir)
    {
        std::string aux;
        switch (graphics[0]->getConstrainedDOF())
        {
        case X:
            aux = "x";
            break;
        case Y:
            aux = "y";
            break;
        case Z:
            aux = "z";
            break;
        default:
            break;
        }
        title = "Node " + std::to_string(graphics[0]->getNodeIndex()) + " Results in " + aux;
        fileName = "node_" + std::to_string(graphics[0]->getNodeIndex()) + "_results_in_" + aux;
    }
    else if (var and dir)
    {
        std::string aux1;
        switch (graphics[0]->getVariable())
        {
        case POSITION:
            aux1 = "Positions";
            break;
        case VELOCITY:
            aux1 = "Velocities";
            break;
        case ACCELERATION:
            aux1 = "Accelerations";
            break;
        case CONTACT_FORCE:
            aux1 = "Contact Forces";
            break;
        default:
            break;
        }
        std::string aux2;
        switch (graphics[0]->getConstrainedDOF())
        {
        case X:
            aux2 = "x";
            break;
        case Y:
            aux2 = "y";
            break;
        case Z:
            aux2 = "z";
            break;
        default:
            break;
        }
        title = aux1 + " in " + aux2;
        if (!aux1.empty())
            aux1[0] = std::tolower(aux1[0]); // Converte apenas o primeiro caractere para minúscula
        fileName = aux1 + "_in_" + aux2;
    }
    else if (node)
    {
        title = "Node " + std::to_string(graphics[0]->getNodeIndex()) + " Results";
        fileName = "node_" + std::to_string(graphics[0]->getNodeIndex()) + "_results";
    }
    else if (var)
    {
        std::string aux;
        switch (graphics[0]->getVariable())
        {
        case POSITION:
            aux = "Positions";
            break;
        case VELOCITY:
            aux = "Velocities";
            break;
        case ACCELERATION:
            aux = "Accelerations";
            break;
        case CONTACT_FORCE:
            aux = "Contact Forces";
            break;
        case STRAIN_ENERGY:
            aux = "Total Energy";
            break;
        case KINETIC_ENERGY:
            aux = "Total Energy";
            break;
        case EXTERNAL_ENERGY:
            aux = "Total Energy";
            break;
        case RIGIDITY_ENERGY:
            aux = "Total Energy";
            break;
        case CONTACT_ENERGY:
            aux = "Total Energy";
            break;
        case TOTAL_ENERGY:
            aux = "Total Energy";
            break;
        default:
            break;
        }
        title = aux;
        if (aux == "Total Energy")
            aux = "total_energy"; // Converte apenas o primeiro caractere para minúscula
        else if (!aux.empty())
            aux[0] = std::tolower(aux[0]); // Converte apenas o primeiro caractere para minúscula
        fileName = aux;
    }
    else if (dir)
    {
        std::string aux;
        switch (graphics[0]->getConstrainedDOF())
        {
        case X:
            aux = "x";
            break;
        case Y:
            aux = "y";
            break;
        case Z:
            aux = "z";
            break;
        default:
            break;
        }
        title = "Results in " + aux;
        fileName = "results_in_" + aux;
    }
}

std::string Gnuplot::getLegend(OutputGraphic *graph, bool test[3])
{
    bool node = test[0];
    bool var = test[1];
    bool dir = test[2];
    std::string aux1, aux2;
    switch (graph->getVariable())
    {
    case POSITION:
        aux1 = "Position";
        break;
    case VELOCITY:
        aux1 = "Velocity";
        break;
    case ACCELERATION:
        aux1 = "Acceleration";
        break;
    case CONTACT_FORCE:
        aux1 = "Contact Force";
        break;
    case STRAIN_ENERGY:
        aux1 = "Strain Energy";
        break;
    case KINETIC_ENERGY:
        aux1 = "Kinetic Energy";
        break;
    case EXTERNAL_ENERGY:
        aux1 = "External Energy";
        break;
    case RIGIDITY_ENERGY:
        aux1 = "Rigidity Energy";
        break;
    case CONTACT_ENERGY:
        aux1 = "Contact Energy";
        break;
    case TOTAL_ENERGY:
        aux1 = "Total Energy";
        break;
    default:
        break;
    }
    switch (graph->getConstrainedDOF())
    {
    case X:
        aux2 = "x";
        break;
    case Y:
        aux2 = "y";
        break;
    case Z:
        aux2 = "z";
        break;
    default:
        break;
    }

    std::string legend;
    if (node and var)
    {
        legend = aux2 + " direction";
    }
    else if (node and dir)
    {
        legend = aux1;
    }
    else if (var and dir)
    {
        legend = "Node " + std::to_string(graph->getNodeIndex());
    }
    else if (node)
    {
        legend = aux1 + " in " + aux2;
    }
    else if (var and (graph->getVariable() == STRAIN_ENERGY or graph->getVariable() == KINETIC_ENERGY or graph->getVariable() == EXTERNAL_ENERGY or graph->getVariable() == RIGIDITY_ENERGY or graph->getVariable() == CONTACT_ENERGY or graph->getVariable() == TOTAL_ENERGY))
    {
        legend = aux1;
    }
    else if (var and (graph->getVariable() == STRAIN_ENERGY or graph->getVariable() == KINETIC_ENERGY or graph->getVariable() == EXTERNAL_ENERGY or graph->getVariable() == RIGIDITY_ENERGY or graph->getVariable() == CONTACT_ENERGY or graph->getVariable() == TOTAL_ENERGY))
    {
        legend = "Node " + std::to_string(graph->getNodeIndex()) + ", " + aux2 + " direction";
    }
    else if (dir)
    {
        legend = "Node " + std::to_string(graph->getNodeIndex()) + ", " + aux1;
    }
    return legend;
}

void Gnuplot::plot()
{
    fprintf(gp, "set terminal pngcairo\n");
    fprintf(gp, "set output \"%s\"\n", outputFile.c_str());

    std::ostringstream cmd;
    cmd << "plot ";
    for (size_t i = 0; i < plots.size(); ++i)
    {
        std::string every;
        if (plots[i].pointStyle != "")
            every = " every " + frequency_;
        cmd << "\"" << plots[i].dataFile << "\"" + every << " using 1:2 title \"" << plots[i].title << "\" with " << plots[i].style << plots[i].pointStyle
            << " lc rgb \"" << plots[i].color << "\"";
        if (i < plots.size() - 1)
        {
            cmd << ", ";
        }
    }
    cmd << "\n";
    fprintf(gp, "%s", cmd.str().c_str());
    fflush(gp);
}

void Gnuplot::view() const
{
    std::string scriptFile = "gnuplot_script.gp";
    std::ofstream script(scriptFile);
    if (!script)
    {
        throw std::runtime_error("Failed to create Gnuplot script file.");
    }

    script << "set terminal qt\n"; // Usa o terminal qt para visualização interativa
    script << viewData;

    script << "plot ";
    for (size_t i = 0; i < plots.size(); ++i)
    {
        script << "\"" << plots[i].dataFile << "\" using 1:2 title \"" << plots[i].title << "\" with " << plots[i].style
               << " lc rgb \"" << plots[i].color << "\"";
        if (i < plots.size() - 1)
        {
            script << ", ";
        }
    }
    script << "\n";
    script << "pause -1 'Press Enter to continue'\n";
    script.close();

    std::string command = "gnuplot -persist " + scriptFile;
    system(command.c_str());

    std::remove(scriptFile.c_str());
}

void Gnuplot::enableGrid(const std::string &axis)
{
    if (axis == "x")
    {
        fprintf(gp, "set grid xtics\n");
        viewData += "set grid xtics\n";
    }
    else if (axis == "y")
    {
        fprintf(gp, "set grid ytics\n");
        viewData += "set grid ytics\n";
    }
    else if (axis == "both")
    {
        fprintf(gp, "set grid\n");
        viewData += "set grid\n";
    }
    else
    {
        throw std::invalid_argument("Invalid axis. Use 'x', 'y', or 'both'.");
    }
}

void Gnuplot::disableGrid(const std::string &axis)
{
    if (axis == "x")
    {
        fprintf(gp, "unset grid xtics\n");
        viewData += "unset grid xtics\n";
    }
    else if (axis == "y")
    {
        fprintf(gp, "unset grid ytics\n");
        viewData += "unset grid ytics\n";
    }
    else if (axis == "both")
    {
        fprintf(gp, "unset grid\n");
        viewData += "unset grid\n";
    }
    else
    {
        throw std::invalid_argument("Invalid axis. Use 'x', 'y', or 'both'.");
    }
}

Gnuplot::Plot::Plot(const std::string &dataFile,
                    const std::string &title,
                    const std::string &style,
                    const std::string &pointStyle,
                    const std::string &color)
    : dataFile(dataFile),
      title(title),
      style(style),
      pointStyle(pointStyle),
      color(color) {}
