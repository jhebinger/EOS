/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2011 Frederik Beaujean
 *
 * This file is part of the EOS project. EOS is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * EOS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <test/test.hh>
#include <eos/utils/observable_set.hh>

#include <vector>

using namespace test;
using namespace eos;

namespace eos
{
    struct TestObservable :
        public Observable
    {
            Parameters p;

            Kinematics k;

            Options o;

            std::string n, mass_name;

            UsedParameter mass;

            TestObservable(const Parameters & p, const Kinematics & k, const std::string & mass_name) :
                p(p),
                k(k),
                o(),
                n("test-observable[" + mass_name + "]"),
                mass_name(mass_name),
                mass(p[mass_name], *this)
            {
            }

            virtual ~TestObservable()
            {
            }

            virtual double evaluate() const
            {
                return mass();
            }

            virtual ObservablePtr clone() const
            {
                return ObservablePtr(new TestObservable(p.clone(), k.clone(), mass_name));
            }

            virtual ObservablePtr clone(const Parameters & parameters) const
            {
                return ObservablePtr(new TestObservable(parameters, k.clone(), mass_name));
            }

            virtual Parameters parameters()
            {
                return p;
            }

            virtual Kinematics kinematics()
            {
                return k;
            }

            virtual Options options()
            {
                return o;
            }

            const std::string & name() const
            {
                return n;
            }

            void set_option(const std::string & key, const std::string & value = "")
            {
                o.set(key, value);
            }
    };
}

class ObservableSetTest :
    public TestCase
{
    public:
        ObservableSetTest() :
            TestCase("observable_set_test")
        {
        }

        virtual void run() const
        {
            // create simple observables vector
            {
                ObservableSet o;
                Parameters p = Parameters::Defaults();
                o.add(ObservablePtr(new TestObservable(p, Kinematics(), "mass::b(MSbar)")));
                o.add(ObservablePtr(new TestObservable(p, Kinematics(), "mass::c")));
                o.add(ObservablePtr(new TestObservable(p, Kinematics(), "mass::s(2GeV)")));

                p["mass::b(MSbar)"] = 4.5;
                p["mass::c"] = 1.5;
                p["mass::s(2GeV)"] = 0.1;

                // evaluate the observables
                std::vector<double> results;
                for (auto i = o.begin(), i_end = o.end(); i != i_end; ++i)
                {
                    results.push_back((**i).evaluate());
                }

                TEST_CHECK_EQUAL(results[0], 4.5);
                TEST_CHECK_EQUAL(results[1], 1.5);
                TEST_CHECK_EQUAL(results[2], 0.1);

                // check iterator
                unsigned counter = 0;
                for (auto i = o.begin(), i_end = o.end(); i != i_end ; ++i)
                    counter++;
                TEST_CHECK_EQUAL(counter, 3);

                // random access
                TEST_CHECK_EQUAL(o[1]->name(), "test-observable[mass::c]");
            }

            // name, options and kinematics need to differ
            {
                ObservableSet o;
                Parameters p = Parameters::Defaults();

                // adding to empty container should always work
                TEST_CHECK(o.add(ObservablePtr(new TestObservable(p, Kinematics(), "mass::b(MSbar)"))).second);

                // identical variable is not added
                TEST_CHECK( ! o.add(ObservablePtr(new TestObservable(p, Kinematics(), "mass::b(MSbar)"))).second);

                TestObservable * obs2 = new TestObservable(p, Kinematics(), "mass::b(MSbar)");
                obs2->set_option("opt", "har");

                // variable with different option than before
                TEST_CHECK(o.add(ObservablePtr(obs2)).second);

                Kinematics kin;
                kin.declare("s", 1.0);

                // different kinematics
                TEST_CHECK(o.add(ObservablePtr(new TestObservable(p, kin, "mass::b(MSbar)"))).second);

                // try the same again
                TEST_CHECK( ! o.add(ObservablePtr(new TestObservable(p, kin, "mass::b(MSbar)"))).second);
            }
        }
} observable_set_test;
