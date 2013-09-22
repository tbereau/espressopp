// ESPP_CLASS
#ifndef _INTERACTION_VSPHERESELFINTERACTIONTEMPLATE_HPP
#define _INTERACTION_VSPHERESELFINTERACTIONTEMPLATE_HPP

#include "mpi.hpp"
#include "Interaction.hpp"
#include "Real3D.hpp"
#include "Tensor.hpp"
#include "Particle.hpp"
#include "iterator/CellListIterator.hpp"
#include "esutil/Array2D.hpp"
#include "bc/BC.hpp"
#include "SystemAccess.hpp"
#include "storage/Storage.hpp"
#include "Interaction.hpp"
#include "types.hpp"

namespace espresso {
  namespace interaction {
  using namespace iterator;
    template < typename _Potential >
    class VSphereSelfInteractionTemplate: public Interaction, SystemAccess {
        
    protected:
      typedef _Potential Potential;
      
    public:
      VSphereSelfInteractionTemplate
      (shared_ptr < System > system,
       shared_ptr < Potential > _potential)
        : SystemAccess(system), potential(_potential)
      {
        if (! potential) {
          LOG4ESPP_ERROR(theLogger, "NULL potential");
        }
      }

      virtual ~VSphereSelfInteractionTemplate() {};

      void
      setPotential(shared_ptr < Potential> _potential) {
        if (_potential) {
          potential = _potential;
        } else {
          LOG4ESPP_ERROR(theLogger, "NULL potential");
        }
      }

      shared_ptr < Potential > getPotential() {
        return potential;
      }

      virtual void addForces();
      virtual real computeEnergy();
      virtual real computeEnergyAA();
      virtual real computeEnergyCG();      
      virtual void computeVirialX(std::vector<real> &p_xx_total, int bins); 
      virtual real computeVirial();
      virtual void computeVirialTensor(Tensor& w);
      virtual void computeVirialTensor(Tensor& w, real z);
      virtual void computeVirialTensor(Tensor *w, int n);
      virtual real getMaxCutoff();
      virtual int bondType() { return Single; }

    protected:
      int ntypes;
      shared_ptr < Potential > potential;
    };

    //////////////////////////////////////////////////
    // INLINE IMPLEMENTATION
    //////////////////////////////////////////////////
    template < typename _Potential > inline void
    VSphereSelfInteractionTemplate < _Potential >::addForces() {
      LOG4ESPP_INFO(theLogger, "compute force of the VSphere Self potential");
      System& system = getSystemRef();
      CellList realCells = system.storage->getRealCells();
      // loop over all particles of the real cells
      Real3D radius = 0.0;
      Real3D force  = 0.0;
      for(CellListIterator cit(realCells); !cit.isDone(); ++cit) {
    	  radius[0] = cit->radius();
          if(potential->_computeForce(force, radius)) {
            cit->fradius() += force[0];
          }
      }
    }
    
    template < typename _Potential > inline real
    VSphereSelfInteractionTemplate < _Potential >::
    computeEnergy() {
      LOG4ESPP_INFO(theLogger, "compute energy of the VSphere Self potential");
      System& system = getSystemRef();
      CellList realCells = system.storage->getRealCells();
      // loop over all particles of the real cells
      real e = 0.0;
      Real3D radius = 0.0;
      for(CellListIterator cit(realCells); !cit.isDone(); ++cit) {
    	radius[0] = cit->radius();
        e += potential->_computeEnergy(radius);
      }
      real esum;
      boost::mpi::all_reduce(*mpiWorld, e, esum, std::plus<real>());
      return esum;
    }
    
    template < typename _Potential > inline real
    VSphereSelfInteractionTemplate < _Potential >::
    computeEnergyAA() {
      std::cout << "Warning! At the moment computeEnergyAA() in VSphereSelfInteractionTemplate does not work." << std::endl;
      return 0.0;
    }
    
    template < typename _Potential > inline real
    VSphereSelfInteractionTemplate < _Potential >::
    computeEnergyCG() {
      std::cout << "Warning! At the moment computeEnergyCG() in VSphereSelfInteractionTemplate does not work." << std::endl;
      return 0.0;
    }
    
    template < typename _Potential >
    inline void
    VSphereSelfInteractionTemplate < _Potential >::
    computeVirialX(std::vector<real> &p_xx_total, int bins) {
        std::cout << "Warning! At the moment computeVirialX in VerletListInteractionTemplate does not work." << std::endl << "Therefore, the corresponding interactions won't be included in calculation." << std::endl;
    }

    template < typename _Potential > inline real
    VSphereSelfInteractionTemplate < _Potential >::
    computeVirial() {
      return 0.0;
    }

    template < typename _Potential > inline void
    VSphereSelfInteractionTemplate < _Potential >::computeVirialTensor(Tensor& w){
      LOG4ESPP_INFO(theLogger, "The virial of the VSphere Self potential is 0");
    }

    template < typename _Potential > inline void
    VSphereSelfInteractionTemplate < _Potential >::
    computeVirialTensor(Tensor& w, real z){
      LOG4ESPP_INFO(theLogger, "The virial of the VSphere Self potential is 0");
    }
    
    template < typename _Potential > inline void
    VSphereSelfInteractionTemplate < _Potential >::
    computeVirialTensor(Tensor *w, int n){
      LOG4ESPP_INFO(theLogger, "The virial of the VSphere Self potential is 0");
    }
    
    template < typename _Potential >
    inline real
    VSphereSelfInteractionTemplate< _Potential >::
    getMaxCutoff() {
      return potential->getCutoff();
    }
  }
}
#endif
