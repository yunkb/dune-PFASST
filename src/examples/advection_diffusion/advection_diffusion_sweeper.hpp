/**
 * @defgroup AdvectionDiffusionFiles Files
 * @ingroup AdvectionDiffusion
 *
 * @file examples/advection_diffusion/advection_diffusion_sweeper.hpp
 * @since v0.1.0
 */
#ifndef _EXAMPLES__ADVEC_DIFF__ADVECTION_DIFFUSION_SWEEPER_HPP_
#define _EXAMPLES__ADVEC_DIFF__ADVECTION_DIFFUSION_SWEEPER_HPP_

#include <cassert>
#include <complex>
#include <cstdlib>
#include <map>
#include <ostream>
#include <vector>
#include <utility>
using namespace std;

#include <pfasst/globals.hpp>
#include <pfasst/logging.hpp>
//#include <pfasst/encap/imex_sweeper.hpp>

//#include "../../sweeper/fe_imex_sweeper.hpp"
#include <pfasst/sweeper/imex.hpp>

using pfasst::encap::Encapsulation;
using pfasst::encap::as_vector;

//#include <pfasst/encap/vector.hpp>

#include "../../datatypes/dune_vec.hpp"

//#include "../../finite_element_stuff/assemble.hpp"
#include "../../finite_element_stuff/fe_manager.hpp"
//#include "fftw_workspace_dft1d.hpp"

#ifndef PI
#define PI 3.1415926535897932385
#endif


namespace pfasst
{
  namespace examples
  {
    /**
     * Advection-Diffusion example
     *
     * @defgroup AdvectionDiffusion Advection Diffusion
     * @ingroup Examples
     *
     * This directory contains several implementations of an advection/diffusion solver using the
     * PFASST framework.
     *
     * All of the solvers use the SDC sweeper defined in `advection_diffusion_sweeper.hpp`.
     *
     * The FFT parts can can be found in `fftw_manager.hpp` and `fftw_workspace.hpp`.
     *
     * The implementations are, in order of complexity:
     *
     *  - `vanilla_sdc.cpp` - basic example that uses an encapsulated IMEX sweeper.
     *
     *  - `serial_mlsdc.cpp` - basic multi-level version that uses polynomial interpolation in time and
     *    spectral interpolation in space, as defined in `specrtal_transfer_1d.hpp`.
     *
     *  - `serial_mlsdc_autobuild.cpp` - same as above, but uses the "auto build" feature to shorten
     *    `main`.
     */
    namespace advection_diffusion
    {
      /**
       * @name Containers for errors/residuals etc.
       * @{
       */
      typedef map<tuple<size_t, size_t>, double> error_map; // step, iteration -> error
      typedef map<size_t, error_map> residual_map; // level, (step, iteration) -> residual
      //! @}

      typedef error_map::value_type vtype;
      typedef error_map::key_type ktype;

      /**
       * advection-diffusion sweeper with semi-implicit time-integration.
       * @ingroup AdvectionDiffusion
       */
      template<typename time = pfasst::time_precision>
      class AdvectionDiffusionSweeper
        : public encap::FeIMEXSweeper<time>
      {
        public:
          static void init_logs()
          {
            pfasst::log::add_custom_logger("Advec");
          }

          using data_type = pfasst::encap::Dune_VectorEncapsulation<double>;

        private:
          //! @{
          //FFTManager<FFTWWorkspaceDFT1D<data_type>> _fft;
          vector<complex<double>> ddx, lap;
          //! @}

          //! @{
          error_map errors;
          error_map residuals;
          //! @}

	  
	  
	  typedef Dune::YaspGrid<1> GridType; 
	  //typedef GridType::LeafGridView GridView;
	  
	  typedef GridType::LevelGridView GridView;
	  using BasisFunction = Dune::Functions::PQkNodalBasis<GridView,BASE_ORDER>;
          //using BasisFunction = Dune::Functions::PQkNodalBasis<GridView,BASE_ORDER>;
          //! @{
          double v  = 1.0;
          time   t0 = 0.0;
          double nu = 0.1; //0.02;
          size_t nf1evals = 0;
	  std::shared_ptr<fe_manager> FinEl;
	  Dune::BCRSMatrix<Dune::FieldMatrix<double,1,1> > M_dune;
          Dune::BCRSMatrix<Dune::FieldMatrix<double,1,1> > A_dune;
	  std::shared_ptr<BasisFunction>  basis;
          //! @}

        public:
          //! @{
          explicit AdvectionDiffusionSweeper( std::shared_ptr<fe_manager> FinEl, size_t nlevel=0)
          {
	    this->FinEl = FinEl;
	    basis = FinEl->get_basis(nlevel);
	    
	    assembleProblem(basis, A_dune, M_dune);
            //this->ddx.resize(nvars);
            //this->lap.resize(nvars);
            //for (size_t i = 0; i < nvars; i++) {
              //double kx = 2 * PI * ((i <= nvars / 2) ? int(i) : int(i) - int(nvars));
              //this->ddx[i] = complex<double>(0.0, 1.0) * kx;
              //this->lap[i] = (kx * kx < 1e-13) ? 0.0 : -kx * kx;
            //}
          }

          AdvectionDiffusionSweeper() = default;

          virtual ~AdvectionDiffusionSweeper()
          {
            ML_CLOG(INFO, "Advec", "number of f1 evals: " << this->nf1evals);
          }
          //! @}

          //! @{
          void exact(shared_ptr<Encapsulation<time>> q, time t)
          {
            this->exact(as_vector<double, time>(q), t);
          }

          void exact(data_type& q, time t)
          {
            /*size_t n = q.size();
            double a = 1.0 / sqrt(4 * PI * nu * (t + t0));

            for (size_t i = 0; i < n; i++) {
              q[i] = 0.0;
            }

            for (int ii = -2; ii < 3; ii++) {
              for (size_t i = 0; i < n; i++) {
                double x = double(i) / n - 0.5 + ii - t * v;
                q[i] += a * exp(-x * x / (4 * nu * (t + t0)));
              }
            }*/
	    const size_t dim=1;		  
	    const double nu=this->nu;
	    const double pi=PI;
	    auto exact_solution = [t, nu, dim, pi](const FieldVector<double,dim>&x){
	      double solution=1.0;
	      for(int i=0; i<dim; i++){solution *= std::sin(pi * x[i]);}
	      return (solution * std::exp(-t * dim * pi *pi * nu));
	    };

	    auto N_x = [t](const FieldVector<double,dim>&x){
	      return x;
	    };

	    BlockVector<FieldVector<double,dim>> x_node;
	    interpolate(*basis, x_node, N_x);

	    interpolate(*basis, q, exact_solution);

	    
          }

          void echo_error(time t)
          {
            auto& qend = as_vector<double, time>(this->get_end_state());
            data_type qex(qend.size());

            this->exact(qex, t);

            double max = 0.0;
            for (size_t i = 0; i < qend.size(); i++) {
              double d = abs(qend[i] - qex[i]);
              if (d > max) { max = d; }
            }

            auto n = this->get_controller()->get_step();
            auto k = this->get_controller()->get_iteration();

            this->errors.insert(vtype(ktype(n, k), max));

            ML_CLOG(INFO, "Advec", "step: " << n << ", iter: " << k << ", error: " << max);
          }

          void echo_residual()
          {
            vector<shared_ptr<Encapsulation<time>>> residuals;

            for (size_t m = 0; m < this->get_nodes().size(); m++) {
                residuals.push_back(this->get_factory()->create(pfasst::encap::solution));
            }
            this->residual(this->get_controller()->get_step_size(), residuals);

            vector<time> rnorms;
            for (auto r: residuals) {
              rnorms.push_back(r->norm0());
            }
            auto rmax = *std::max_element(rnorms.begin(), rnorms.end());

            auto n = this->get_controller()->get_step();
            auto k = this->get_controller()->get_iteration();

            this->residuals[ktype(n, k)] = rmax;

            ML_CLOG(INFO, "Advec", "step: " << n << ", iter: " << k << ", resid: " << rmax);
          }

          error_map get_errors()
          {
            return this->errors;
          }

          error_map get_residuals()
          {
            return this->residuals;
          }
          //! @}

          //! @{
          /**
           * @copybrief pfasst::encap::IMEXSweeper::predict()
           */
          void post_predict() override
          {
            time t  = this->get_controller()->get_time();
            time dt = this->get_controller()->get_step_size();
            this->echo_error(t + dt);
            this->echo_residual();
          }

          /**
           * @copybrief pfasst::encap::IMEXSweeper::sweep()
           */
          void post_sweep() override
          {
            time t  = this->get_controller()->get_time();
            time dt = this->get_controller()->get_step_size();
            this->echo_error(t + dt);
            this->echo_residual();
          }
          //! @}

          //! @{
          /**
           * @copybrief pfasst::encap::IMEXSweeper::f_expl_eval()
           */
          void f_expl_eval(shared_ptr<Encapsulation<time>> f_expl_encap,
                           shared_ptr<Encapsulation<time>> u_encap,
                           time t) override
          {
	
            UNUSED(t);
            //auto& u = as_vector<double, time>(u_encap);
            auto& f_expl = as_vector<double, time>(f_expl_encap);
	    f_expl.zero();
	    this->nf1evals++;	    
            //double c = -v / double(u.size());

            /*auto* z = this->_fft.get_workspace(u.size())->forward(u);
            for (size_t i = 0; i < u.size(); i++) {
              z[i] *= c * this->ddx[i];
            }
            this->_fft.get_workspace(f_expl.size())->backward(f_expl);

            this->nf1evals++;*/
	   
          }

          /**
           * @copybrief pfasst::encap::IMEXSweeper::f_impl_eval()
           */
          void f_impl_eval(shared_ptr<Encapsulation<time>> f_impl_encap,
                           shared_ptr<Encapsulation<time>> u_encap,
                           time t) override
          {
	    
            UNUSED(t);

            auto& u = as_vector<double, time>(u_encap);
            auto& f_impl = as_vector<double, time>(f_impl_encap);
 

	    
	    encap::Dune_VectorEncapsulation<double,double> rhs(u.N());
	    encap::Dune_VectorEncapsulation<double,double> rhs2(u.N());

	    A_dune.mmv(u, rhs);
	    //A_dune.mmv(u, rhs2);

	    /*for (size_t i = 0; i < u.N(); i++) {
	      std::cout << "rhs " << rhs[i] << std::endl;
	    }*/


	    auto DirichletValues = [] (auto x) {return (x[0]<1e-8 or x[0]>0.999999) ? 0 : x[0];};

	    /*for (size_t i = 0; i < u.N(); i++) {
	      for (size_t j = 0; i < u.N(); j++) {
		if(M_dune.exists(i,j)) std::cout << M_dune[i][j] << " ";
	      }
	      std::cout <<  std::endl;
	    }*/	    

	    Dune::MatrixAdapter<MatrixType,VectorType,VectorType> linearOperator(M_dune);

	    Dune::SeqILU0<MatrixType,VectorType,VectorType> preconditioner(M_dune,1.0);

	    Dune::CGSolver<VectorType> cg(linearOperator,
                                preconditioner,
                                1e-10, // desired residual reduction factor
                                500,    // maximum number of iterations
                                0);    // verbosity of the solver
	    Dune::InverseOperatorResult statistics ;

	    cg.apply(f_impl, rhs , statistics );


	    f_impl *= nu;
	    /*for (size_t i = 0; i < u.N(); i++) {
	      std::cout << "impl_eval " << f_impl[i] << std::endl;
	    }*/


	    
          }

          /**
           * @copybrief pfasst::encap::IMEXSweeper::impl_solve()
           */
          void impl_solve(shared_ptr<Encapsulation<time>> f_impl_encap,
                          shared_ptr<Encapsulation<time>> u_encap,
                          time t, time dt,
                          shared_ptr<Encapsulation<time>> rhs_encap) override
          {
            UNUSED(t);
            auto& u = as_vector<double, time>(u_encap);
            auto& f_impl = as_vector<double, time>(f_impl_encap);
            auto& rhs = as_vector<double, time>(rhs_encap);

	    
	    Dune::BlockVector<Dune::FieldVector<double,1> > M_rhs_dune ;
	    M_rhs_dune.resize(rhs.N());

	    M_dune.mv(rhs, M_rhs_dune);

	    Dune::BCRSMatrix<Dune::FieldMatrix<double,1,1> > M_dtA_dune = 	Dune::BCRSMatrix<Dune::FieldMatrix<double,1,1> >(A_dune);
	    M_dtA_dune *= (dt * this->nu);
	    M_dtA_dune += M_dune;

	    Dune::MatrixAdapter<MatrixType,VectorType,VectorType> linearOperator(M_dtA_dune);

	    Dune::SeqILU0<MatrixType,VectorType,VectorType> preconditioner(M_dtA_dune,1.0);

	    Dune::CGSolver<VectorType> cg(linearOperator,
                                preconditioner,
                                1e-10, // desired residual reduction factor
                                500,    // maximum number of iterations
                                0);    // verbosity of the solver

	    Dune::InverseOperatorResult statistics ;

	    cg.apply(u, M_rhs_dune , statistics ); //rhs ist nicht constant!!!!!!!!!


	    for (size_t i = 0; i < u.N(); i++) {
	      f_impl[i] = (u[i] - rhs[i]) / (dt);
	      //std::cout << "impl_solve " << f_impl[i] << std::endl;
	    }
	    
	    //f_impl_eval(f_impl_encap,u_encap,t);
	    //std::exit(0);

	    //this->_num_impl_solves++;
	    
	    
	    
            //double c = nu * double(dt);

            /*auto* z = this->_fft.get_workspace(rhs.size())->forward(rhs);
            for (size_t i = 0; i < u.size(); i++) {
              z[i] /= (1.0 - c * this->lap[i]) * double(u.size());
            }
            this->_fft.get_workspace(u.size())->backward(u);

            for (size_t i = 0; i < u.size(); i++) {
              f_impl[i] = (u[i] - rhs[i]) / double(dt);
            }*/
          }
          //! @}
      };
    }  // ::pfasst::examples::advection_diffusion
  }  // ::pfasst::examples
}  // ::pfasst

#endif // _EXAMPLES__ADVEC_DIFF__ADVECTION_DIFFUSION_SWEEPER_HPP_