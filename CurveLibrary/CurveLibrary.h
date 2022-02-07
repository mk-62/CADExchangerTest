/*
файл:	CurveLibrary.h

автор:	яковлев јлексей ¬€чеславович
дата:	7 феврал€ 2022 года

ќпределени€ классов библиотеки кривых дл€ тестового задани€ от CADExchanger.
*/

#pragma once

#if CURVELIBRARY_EXPORTS
    #define DLLIMPORT __declspec (dllexport)
#else 
    #define DLLIMPORT __declspec (dllimport)
#endif

#include <stdexcept>

namespace testwork
{

    struct V3d
    {
        V3d() = default;
        double x;
        double y;
        double z;
    };

    class invalid_radius : public std::invalid_argument
    {
    public:
        invalid_radius() : std::invalid_argument("invalid radius value") { }
    };

    class invalid_step : public std::invalid_argument
    {
    public:
        invalid_step() : std::invalid_argument("invalid step value") { }
    };

    class DLLIMPORT Curve
    {
    public:
        Curve() = default;

        virtual V3d Point(double t) = 0;
        virtual V3d FirstDerivative(double t) = 0;

        virtual void(*Id() const)() = 0;
        virtual ~Curve() noexcept = default;
    };

    class DLLIMPORT Circle : public Curve
    {
    protected:
        double m_radius;
    public:
        Circle(double radius) : m_radius(radius) 
            { if(radius<=0) throw invalid_radius(); }

        double getRadius() const { return m_radius; }
        virtual V3d Point(double t);
        virtual V3d FirstDerivative(double t);

        static void SelfType();
        void(*Id() const)() override { return &Circle::SelfType; }
    };

    class DLLIMPORT Ellipse : public Curve
    {
    protected:
        double m_radius_a;
        double m_radius_b;
    public:
        Ellipse(double radius_a, double radius_b) : m_radius_a(radius_a), m_radius_b(radius_b) 
            { if(radius_a<=0 || radius_b<=0) throw invalid_radius(); }

        double getRadiusA() const { return m_radius_a; }
        double getRadiusB() const { return m_radius_b; }
        virtual V3d Point(double t);
        virtual V3d FirstDerivative(double t);

        static void SelfType();
        void(*Id() const)() override { return &Ellipse::SelfType; }
    };

    class DLLIMPORT Helix : public Curve
    {
    protected:
        double m_radius;
        double m_step;
    public:
        Helix(double radius, double step) : m_radius(radius), m_step(step) 
            { if(radius<=0) throw invalid_radius(); if(step==0)throw invalid_step(); }

        double getRadius() const { return m_radius; }
        double getStep() const { return m_step; }
        virtual V3d Point(double t);
        virtual V3d FirstDerivative(double t);

        static void SelfType();
        void(*Id() const)() override { return &Helix::SelfType; }
    };

}
