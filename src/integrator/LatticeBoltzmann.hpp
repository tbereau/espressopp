/*
  Copyright (C) 2012,2013
      Max Planck Institute for Polymer Research
  Copyright (C) 2008,2009,2010,2011
      Max-Planck-Institute for Polymer Research & Fraunhofer SCAI
  
  This file is part of ESPResSo++.
  
  ESPResSo++ is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  ESPResSo++ is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

// ESPP_CLASS
#ifndef _INTEGRATOR_LATTICEBOLTZMANN_HPP
#define _INTEGRATOR_LATTICEBOLTZMANN_HPP

//#include "boost/serialization/vector.hpp"
#include "logging.hpp"
#include "Extension.hpp"
#include "boost/signals2.hpp"
#include "esutil/Timer.hpp"
#include "Real3D.hpp"
#include "Int3D.hpp"
#include "LatticeSite.hpp"

namespace espressopp {
	namespace integrator {
		class LatticeBoltzmann : public Extension {
      /* LatticeBoltzmann constructor expects 5 parameters (and a system pointer).
			These are: lattice size in 3D Ni, lattice spacing a, lattice timestep tau,
			number of dimensions and number of velocity vectors on a lattice site.
			The lattice size, Ni, is an obligatory parameter and must be set at the
			beginning of the simulation.

			The default lattice model is D3Q19 and both lattice spacing and timestep
			are set to 1.
			
			Note that at the present stage of development we aim at D3Q19 model.
			If you want to use something else, please, feel free to modify the code.
			
			Originally, we had planned this module to operate in 3D only, so if you
			need a 2D version, there is a bit more tuning involved. On the other hand,
			adding different 3D lattice models (such as D3Q15 or D3Q27) is rather
			straightforward.
		*/
		public:
			LatticeBoltzmann (shared_ptr< System > _system,
												Int3D _nodeGrid, Int3D _Ni, real _a, real _tau, int _numDims, int _numVels);
      ~LatticeBoltzmann ();

      /* SET AND GET DECLARATION */
			void setNodeGrid(Int3D _nodeGrid);		// set processors' arrangement in nodeGrid
			Int3D getNodeGrid();									// get nodeGrid
			
      void setNi(Int3D _Ni);								// set lattice size in x,y and z-directions
      Int3D getNi();												// get lattice size in x,y and z-directions

			void setA (real _a);									// set lattice spacing
			real getA ();													// get lattice spacing

			void setTau (real _tau);							// set lattice timestep
      real getTau ();												// get lattice timestep

			void setGammaB (real _gamma_b);				// set gamma for bulk
			real getGammaB ();										// get gamma for bulk

			void setGammaS (real _gamma_s);				// set gamma for shear
			real getGammaS ();										// get gamma for shear

			void setGammaOdd (real _gamma_odd);		// set gamma odd
			real getGammaOdd ();									// get gamma odd

			void setGammaEven (real _gamma_even);	// set gamma even
			real getGammaEven ();									// get gamma even
			
			void setViscB (real _visc_b);				// set bulk viscosity
			real getViscB ();										// get bulk viscosity
			
			void setViscS (real _visc_s);				// set shear viscosity
			real getViscS ();										// get shear viscosity
			
			void setExtForceFlag (int _extForceFlag);	// set a flag for external force
			int getExtForceFlag ();								// get a flag for external force

			void setNumDims (int _numDims);				// set number of dimensions
			int getNumDims ();										// get number of dimensions

			void setNumVels (int _numVels);				// set number of velocities
			int getNumVels ();										// get number of velocities

			void setStepNum (int _step);					// set current step number
			int getStepNum ();										// get current step number
			
			void setNBins (int _nBins);						// set number of bins
			int getNBins ();											// get number of bins

			void setDistr (int _i, real _distr);	// set distribution
			real getDistr (int _i);								// get distribution
			void incDistr (int _i);								// increment distribution
			
//			void calcHistogram (int _i, int _j, int _k);// calculate the histogram

			void setLBTemp (real _lbTemp);				// set LB-temperature
			real getLBTemp ();										// get LB-temperature

			void setLBTempFlag (int _lbTempFlag);	// set a flag for fluctuations
			int getLBTempFlag ();									// get a flag for fluctuations

			void setEqWeight (int _l, real _value);	// set eq.weights
			real getEqWeight (int _l);						// get eq.weights

			void setCi (int _l, Real3D _vec);			// set c_i's
			Real3D getCi (int _l);								// get c_i's

			void setCs2 (real _cs2);							// set cs2
			real getCs2 ();												// get cs2

			void setInvB (int _l, real _value);		// set inverse b_i's
			real getInvB (int _l);								// get inverse b_i's

			void setPhi (int _l, real _value);		// set phi for fluctuations
			real getPhi (int _l);									// get phi for fluctuations

			void setLBFluid (Int3D _Ni, int _l, real _value);
			real getLBFluid (Int3D _Ni, int _l);

			void setExtForceLoc (Int3D _Ni, Real3D _extForceLoc);
			Real3D getExtForceLoc (Int3D _Ni);
			void addExtForceLoc (Int3D _Ni, Real3D _extForceLoc);

			void setCouplForceFlag (int _couplForceFlag); // set a flag for a coupling force
			int getCouplForceFlag ();											// get a flag for a coupling force
			
			void readCouplForces ();								// reades coupling forces acting on MD particles at restart
			void saveCouplForces ();								// writes coupling forces acting on MD particles for restart
			
			void setGhostFluid (Int3D _Ni, int _l, real _value);
			/* END OF SET AND GET DECLARATION */

			/* FUNCTIONS DECLARATION */
			void initLatticeModel ();								// initialize lattice model (weights, cis)
			void initGammas (int _idGamma);					// (re)initialize gammas
			void initFluctuations ();								// (re)initialize fluctuation parameters
			void makeLBStep ();											// perform one step of LB
			
			/* COUPLING TO THE MD */
			void setStart(int _start);							// set start indicator for coupling
			int getStart();													// get start indicator
			
			void setCopyTimestep(real _copyTimestep);// set timestep (just a copy of MD timestep)
			real getCopyTimestep();													// get timestep

			/* FIND AND MANIPULATE CENTER-OF-MASS VELOCITY OF MD AND LB */
			Real3D findCMVelMD(int _id);						// find velocity of center of mass of MD particles
			void zeroMDCMVel();											// kill drift velocity of the center of mass
			void galileanTransf(Real3D _specCmVel);	// galilean transform by amount of _momPerPart
			
			/* COUPLING TO MD PARTICLES */
			void coupleLBtoMD();
			void calcRandForce(class Particle&);
			void restoreLBForces();									// restore LB-forces from previous timestep to act onto MD particles
//		void calcInterVel(class Particle&);
			void calcViscForce(class Particle&);
//		void extrapMomToNodes(class Particle&, real _timestep);
			void calcDenMom ();
			real convMDtoLB (int _opCode);
			
			void setFricCoeff (real _fricCoeff);		// set friction coefficient of MD to LB coupling
			real getFricCoeff ();										// get friction coefficient of MD to LB coupling
			
			void setNSteps (int _nSteps);						// set number of md steps between 1 lb
			int getNSteps ();												// get number of md steps between 1 lb

			void setTotNPart (int _totNPart);				// set total number of md particles in the whole system (sum over CPUs)
			int getTotNPart ();											// get total number of md particles in the whole system (sum over CPUs)
			
			void setFOnPart (int _id, Real3D _fOnPart);
			Real3D getFOnPart (int _id);
			void addFOnPart (int _id, Real3D _fOnPart);
			
			void setInterpVel (Real3D _interpVel);
			Real3D getInterpVel ();
			void addInterpVel (Real3D _interpVel);
			
			/* UNIT CONVERSION */
			real convMassMDtoLB();
			real convTimeMDtoLB();
			real convLenMDtoLB();
			
			void collideStream ();									// use collide-stream scheme

			void streaming (int _i, int _j, int _k); // streaming along the velocity vectors

			/* MPI FUNCTIONS */
			void findMyNeighbours ();
			void commHalo ();							// communicate populations in halo regions to the neighbouring CPUs
			void copyForcesFromHalo ();		// copy coupling forces from halo regions to the real lattice sites
			void copyDenMomToHalo ();			// copy den and j from real lattice sites to halo
			void makeDecompose ();				// decompose storage to put escaped real particles into neighbouring CPU
			
			void setMyNeighbour (int _dir, int _rank);
			int getMyNeighbour (int _dir);

			void setHaloSkin (int _haloSkin);
			int getHaloSkin ();

			void setMyNi(Int3D _myNi);		// set lattice size of current cpu + halo in 3 dimensions
			Int3D getMyNi();							// get lattice size of current cpu + halo in 3 dimensions
			
			void setMyPosition(Int3D _myPosition);
			Int3D getMyPosition();
      
			void setMyLeft(Real3D _myLeft);	// set left border of a physical ("real") domain of a CPU
			Real3D getMyLeft();						// get left border of a physical ("real") domain of a CPU
			
			/* control functions */
			void computeDensity (int _i, int _j, int _k);
			void computeMomentum (int _i, int _j, int _k);
			/* END OF FUNCTIONS DECLARATION */

			/** Register this class so it can be used from Python. */
			static void registerPython();

		private:
			int numDims;									// number of dimensions
			int numVels;									// number of velocities
			real cs2;											// squared speed of sound
			real invCs2;									// inverse square of speed of sound
			real a;												// lattice spacing
			real tau;											// lattice timestep
			real gamma_b;
			real gamma_s;
			real gamma_odd;
			real gamma_even;
			real visc_b, visc_s;
			real lbTemp;
			real fricCoeff;
			int start;
			int lbTempFlag;
			int stepNum;									// step number
			int nSteps;
			std::vector<Real3D> c_i;			// velocity vectors
			std::vector<real> eqWeight;		// equilibrium weights
			std::vector<real> inv_b;			// back-transformation weights
			std::vector<real> phi;				// amplitudes of fluctuations
			int totNPart;									// total number of MD particles
			real copyTimestep;							// copy of the MD timestep
			
			Int3D partBin;								// bins where the MD particle is
			int extForceFlag;							// flag for an external force
			int couplForceFlag;						// flag for a coupling force
			std::vector<Real3D> fOnPart;	// force acting onto an MD particle
			Int3D Ni;											// lattice lengths in 3D
			
			int idX, idY, idZ, index;			// indexes in 3D and aligned 1D index
			
			/*	two lattices. lbfluid has f,m and meq. ghostlat has f only.
			*		the latter one used for sake of simplicity during streaming
			*/
//			std::vector< std::vector< std::vector<LBPop> > > lbpop;
			std::vector< std::vector< std::vector<LBSite> > > lbfluid;
			std::vector< std::vector< std::vector<GhostLattice> > > ghostlat;

			int nBins;
			std::vector<real> distr;

			shared_ptr< esutil::RNG > rng;  //!< random number generator used for fluctuations
			
			// COUPLING
			Real3D interpVel;							// interpolated fluid vel at the MD particle position

			// MPI THINGS
			std::vector<int> myNeighbour;
			Int3D myPosition;
			int haloSkin;
			Int3D myNi;
			Int3D nodeGrid;								// 3D-array of processors
			Real3D myLeft;								// left border of a physical ("real") domain for a CPU
			
			// SIGNALS
			boost::signals2::connection _befIntV;
			boost::signals2::connection _recalc2;
			
			// TIMERS
//			esutil::WallTimer timeCollStream;  //!< used for timing
//			esutil::WallTimer timeComm;  //!< used for timing
//			esutil::WallTimer timeCouple;  //!< used for timing
			esutil::WallTimer swapping, colstream;
			real time_sw, time_colstr;
			
			void connect();
			void disconnect();

			/** Logger */
			static LOG4ESPP_DECL_LOGGER(theLogger);
    };
  }
}

#endif
