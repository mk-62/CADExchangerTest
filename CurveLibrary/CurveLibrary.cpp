/*
файл:	CurveLibrary.cpp

автор:	Яковлев Алексей Вячеславович
дата:	7 февраля 2022 года

Реализация классов библиотеки кривых для тестового задания от CADExchanger.
*/

#include "pch.h"
#include "CurveLibrary.h"
#include <iostream>
#include <cmath>

namespace
{
    const double PI = std::atan(1.0)*4;
}
using namespace testwork;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

V3d Circle::Point(double t) { return { m_radius*cos(t), m_radius*sin(t), 0}; }
V3d Circle::FirstDerivative(double t) { return { -m_radius*sin(t), m_radius*cos(t), 0}; }
void Circle::SelfType() { std::cout << "Circle"; }

V3d Ellipse::Point(double t) { return { m_radius_a*cos(t), m_radius_b*sin(t), 0}; }
V3d Ellipse::FirstDerivative(double t) { return { -m_radius_a*sin(t), m_radius_b*cos(t), 0}; }
void Ellipse::SelfType() { std::cout << "Ellipse"; }

V3d Helix::Point(double t) { return { m_radius*cos(t), m_radius*sin(t), m_step*t/2/PI }; }
V3d Helix::FirstDerivative(double t) { return { -m_radius*sin(t), m_radius*cos(t), m_step/2/PI }; }
void Helix::SelfType() { std::cout << "Helix"; }
