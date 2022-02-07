/*
файл:	CADExTest.cpp

автор:	Яковлев Алексей Вячеславович
дата:	7 февраля 2022 года

Основной текст программы тестового задания для CADExchanger.
*/

#include "pch.h"
#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>

#define CURVES      20
#define RADIUS_MIN  1.0
#define RADIUS_MAX  10.0
#define STEP_MIN    1.0
#define STEP_MAX    10.0

static_assert(RADIUS_MIN>0 && RADIUS_MIN<=RADIUS_MAX,"Invalid Circle radius!");
static_assert((STEP_MIN>0 || STEP_MAX<0) && STEP_MIN<=STEP_MAX,"Invalid Helix step!");

namespace testwork
{
    const double PI = std::atan(1.0)*4;

    std::vector<std::shared_ptr<Curve>> static GenerateCurves(int count,
        double radius_min, double radius_max, double step_min, double step_max)
    {
        std::vector<std::shared_ptr<Curve>> container;

        if(count<0) throw std::out_of_range("invalid figures count");
        if(radius_min<=0 || radius_min>radius_max)throw invalid_radius();
        if(step_min<=0 && step_max>=0 || step_min>step_max)throw invalid_step();

        std::random_device rd;
        std::default_random_engine random_engine(rd());
        std::uniform_int_distribution<> figures_dist(0, 2);
        std::uniform_real_distribution<> radius_dist(radius_min,radius_max);
        std::uniform_real_distribution<> step_dist(step_min,step_max);

        container.reserve((size_t)count);
        for(int i=0;i<count;++i)switch (figures_dist(random_engine))
        {
        case 0: 
            container.emplace_back(std::make_shared<Circle>(
                radius_dist(random_engine))); break;
        case 1: 
            container.emplace_back(std::make_shared<Ellipse>(
                radius_dist(random_engine),radius_dist(random_engine))); break;
        case 2: 
            container.emplace_back(std::make_shared<Helix>(
                radius_dist(random_engine),step_dist(random_engine))); break;
        default: throw std::logic_error("invalid case");
        }

        return container;
    }

    std::vector<std::shared_ptr<Circle>> 
        static ExtractSortedCircles(const std::vector<std::shared_ptr<Curve>> &source)
    {
        std::vector<std::shared_ptr<Circle>> container;

        for(const std::shared_ptr<Curve>& fig : source)
            if(fig->Id()==&Circle::SelfType)container.emplace_back(std::static_pointer_cast<Circle>(fig));
        std::sort(container.begin(),container.end(),
            [](const auto &one, const auto &too) { return one->getRadius()<too->getRadius(); });

        return container;
    }
}

using namespace testwork;

int main()
{
    int retcode;

    try 
    {
        std::vector<std::shared_ptr<Curve>> first_container = 
            GenerateCurves(CURVES,RADIUS_MIN,RADIUS_MAX,STEP_MIN,STEP_MAX);

        for(const std::shared_ptr<Curve>& fig : first_container)
        {
            V3d pt = fig->Point(PI/4), dvt = fig->FirstDerivative(PI/4);
            (*fig->Id())(); std::cout << " point: {" << pt.x << ", " << pt.y <<", " << pt.z << "}; ";
            std::cout << "first derivative: {" << dvt.x << ", " << dvt.y << ", " << dvt.z << "};" << std::endl;
        }

        std::vector<std::shared_ptr<Circle>> second_container = ExtractSortedCircles(first_container);

        double radii_sum = 0; int i;
        #pragma omp parallel private(i) shared(second_container, radii_sum)
        {
            #pragma omp for reduction (+: radii_sum)
            for(i=0; i<(int)second_container.size(); ++i)
                radii_sum+=second_container[i]->getRadius();
        }
        std::cout << "sum of circles radii: " << radii_sum << std::endl;
        retcode=0;
    } 
    catch (std::exception const& ex) { std::cerr << ex.what() << std::endl; retcode=-1; }
    catch (...) { std::cerr << "unknown error" << std::endl; retcode=-1; }

    return retcode;
}