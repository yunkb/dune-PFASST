#include <dune/istl/matrixmarket.hh>
#include <fenv.h>

#include <memory>
#include <stdexcept>
using std::shared_ptr;
#include "dune_includes"

#include <pfasst.hpp>
#include <pfasst/quadrature.hpp>
//#include <pfasst/encap/dune_vec.hpp>
#include <pfasst/controller/sdc.hpp>
#include "FE_heat2d_sweeper.hpp"
#include "../../datatypes/dune_vec.hpp"
#include "spectral_transfer.hpp"



#include <iostream>

#include <vector>

using std::shared_ptr;

using encap_traits_t = pfasst::encap::dune_vec_encap_traits<double, double, 1>;

//////////////////////////////////////////////////////////////////////////////////////
//
// Compiletimeparameter
//
//////////////////////////////////////////////////////////////////////////////////////

const size_t DIM = 2;            //Räumliche Dimension des Rechengebiets

const size_t BASIS_ORDER = BASE_ORDER;    //maximale Ordnung der Lagrange Basisfunktionen

//////////////////////////////////////////////////////////////////////////////////////
const size_t nelements = 10;

namespace pfasst
{
  namespace examples
  {
    namespace heat_FE
    {
      using sweeper_t = Heat2d_FE<pfasst::sweeper_traits<encap_traits_t>>;
      using pfasst::transfer_traits;
      using pfasst::contrib::SpectralTransfer;
      using pfasst::SDC;
      using pfasst::quadrature::QuadratureType;
      using heat_FE_sdc_t = SDC<SpectralTransfer<transfer_traits<sweeper_t, sweeper_t, 1>>>;

      shared_ptr<heat_FE_sdc_t> run_sdc(const size_t nelements, const size_t basisorder, const size_t dim, const size_t nnodes,
                                       const QuadratureType& quad_type, const double& t_0,
                                       const double& dt, const double& t_end, const size_t niter)
      {
        typedef sweeper_t::GridType GridType;
        using pfasst::quadrature::quadrature_factory;

        auto sdc = std::make_shared<heat_FE_sdc_t>();
        std::shared_ptr<GridType > mlsdcGrid(sweeper_t::createGrid(nelements));
        auto FinEl = make_shared<fe_manager>(mlsdcGrid, nelements, 2); 
        //sdc->grid_builder(nelements);

        auto sweeper = std::make_shared<sweeper_t>(nelements, basisorder, 0);

        sweeper->quadrature() = quadrature_factory<double>(nnodes, quad_type);

        sdc->add_sweeper(sweeper);

        sdc->set_options();

        sdc->status()->time() = t_0;
        sdc->status()->dt() = dt;
        sdc->status()->t_end() = t_end;
        sdc->status()->max_iterations() = niter;

        sdc->setup();

        //sweeper->initial_state() = sweeper->exact(sdc->get_status()->get_time());
	sweeper->initial_state() = sweeper->initial();

        sdc->run();

        sdc->post_run();


        if(BASIS_ORDER==1) {
          auto grid = (*sweeper).get_grid();
          typedef GridType::LeafGridView GridView;
          GridType::LeafGridView gridView = grid->leafGridView();
          Dune::VTKWriter<GridView> vtkWriter(gridView);
          typedef Dune::BlockVector<Dune::FieldVector<double, 1> > VectorType;
          VectorType x = sweeper->get_end_state()->data();
          //VectorType y = sweeper->exact(t_end)->data();
          //VectorType z = sweeper->initial_state()->data();
          vtkWriter.addVertexData(x, "fe_solution");
          //vtkWriter.addVertexData(y, "exact_solution");
          //vtkWriter.addVertexData(z, "initial_data");

          vtkWriter.write("heat2dMoving_sdc_result");
        }

        sweeper->states()[sweeper->get_states().size()-1]->scaled_add(-1.0 , sweeper->exact(t_end));
        std::cout << sweeper->states()[sweeper->get_states().size()-1]->norm0()<<  std::endl ;



        return sdc;
      }
    }
  }
}



#ifndef PFASST_UNIT_TESTING
  int main(int argc, char** argv) {
    using pfasst::config::get_value;
    using pfasst::quadrature::QuadratureType;
    using pfasst::examples::heat_FE::Heat2d_FE;

    using sweeper_t = Heat2d_FE<pfasst::examples::heat_FE::dune_sweeper_traits<encap_traits_t, BASIS_ORDER, DIM>>;

    pfasst::init(argc, argv, sweeper_t::init_opts);
    pfasst::config::read_commandline(argc, argv);

    const size_t nelements = get_value<size_t>("--num_elements", 1000); //Anzahl der Elemente pro Dimension
    const size_t nnodes    = get_value<size_t>("num_nodes", 3);
    const QuadratureType quad_type = QuadratureType::Uniform_Right;
    const double t_0 = 0.0;
    double dt = get_value<double>("dt", 0.025);
    double t_end = get_value<double>("--tend", 0.1);
    size_t nsteps = get_value<size_t>("--num_steps", 0);
    if (t_end == -1 && nsteps == 0) {
      ML_CLOG(ERROR, "USER", "Either t_end or num_steps must be specified.");
      throw std::runtime_error("either t_end or num_steps must be specified");
    } else if (t_end != -1 && nsteps != 0) {      
      dt = t_end / nsteps;
#if 0
      if (!pfasst::almost_equal(t_0 + nsteps * dt, t_end)) {
        ML_CLOG(ERROR, "USER", "t_0 + nsteps * dt != t_end ("
                               << t_0 << " + " << nsteps << " * " << dt << " = " << (t_0 + nsteps * dt)
                               << " != " << t_end << ")");
        throw std::runtime_error("t_0 + nsteps * dt != t_end");
      }
#endif
    } else if (nsteps != 0) {
      t_end = t_0 + dt * nsteps;
    }
    const size_t niter = get_value<size_t>("--num_iters", 10);

    pfasst::examples::heat_FE::run_sdc(nelements, BASIS_ORDER, DIM, nnodes, quad_type, t_0, dt, t_end, niter);

  }

#endif
