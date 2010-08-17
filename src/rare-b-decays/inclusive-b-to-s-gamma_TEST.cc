/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <test/test.hh>
#include <src/rare-b-decays/inclusive-b-to-s-gamma.hh>

#include <cmath>
#include <limits>

#include <iostream>

using namespace test;
using namespace wf;

class BToXsGammaLargeRecoilTest :
    public TestCase
{
    public:
        BToXsGammaLargeRecoilTest() :
            TestCase("b_to_x_s_gamma_minimal_test")
        {
        }

        virtual void run() const
        {
            /* Minimal */

            // Standard Model
            {
                Parameters p = Parameters::Defaults();
                p["Abs{c7}"] = +0.331;
                p["Arg{c7}"] = M_PI;
                p["c8"] = -0.181;
                p["Abs{c9}"] = +4.27;
                p["Arg{c9}"] = 0.0;
                p["Abs{c10}"] = +4.173;
                p["Arg{c10}"] = M_PI;

                ObservableOptions oo;

                BToXsGamma<Minimal> d(p, oo);

                const double eps = 1e-9;

                TEST_CHECK_NEARLY_EQUAL(3.86877e-4, d.integrated_branching_ratio(), eps);
            }

            // Zero test
            {
                Parameters p = Parameters::Defaults();
                p["Abs{c7}"] = +0.3;
                p["Arg{c7}"] = M_PI;
                p["c8"] = -0.181;
                p["Abs{c9}"] = +4.27;
                p["Arg{c9}"] = 0.0;
                p["Abs{c10}"] = +4.173;
                p["Arg{c10}"] = M_PI;

                ObservableOptions oo;

                BToXsGamma<Minimal> d(p, oo);

                const double eps = 1e-9;

                TEST_CHECK_NEARLY_EQUAL(3.15e-4, d.integrated_branching_ratio(), eps);
            }
        }
} b_to_x_s_gamma_large_recoil_test;
