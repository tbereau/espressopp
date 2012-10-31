// ESPP_CLASS
#ifndef _FIXEDTRIPLECOSLIST_HPP
#define _FIXEDTRIPLECOSLIST_HPP

#include "log4espp.hpp"
#include "types.hpp"

#include "Particle.hpp"
#include "esutil/ESPPIterator.hpp"
//#include <boost/unordered_map.hpp>
#include <map>
#include <boost/signals2.hpp>
//#include "FixedListComm.hpp"
#include "SystemAccess.hpp"

/*
 * This list is temporary solution. In general it should be derived from FixedTripleList.
 * in order not to generate similar code.
 * 
 * It will store the initial angle for each triple
 */

using namespace std;

namespace espresso {
  class FixedTripleCosList : public TripleList, SystemAccess{
      protected:
		boost::signals2::connection con1, con2, con3;
		typedef multimap <longint,pair<pair<longint, longint>, real> > TriplesCos;
		TriplesCos triplesCos;
		using TripleList::add;

	  public:
		FixedTripleCosList( shared_ptr< System > system ); //shared_ptr<storage::Storage> _storage
		virtual ~FixedTripleCosList();
		//bool add(pvec pids) { _comm.add(pids); }
		/** Add the given particle triple to the list on this processor if the
		particle with the lower id belongs to this processor.  Note that
		this routine does not check whether the triple is inserted on
		another processor as well.
		\return whether the triple was inserted on this processor.
		*/
		virtual bool add(longint pid1, longint pid2, longint pid3);
		virtual void beforeSendParticles(ParticleList& pl, class OutBuffer &buf);
		void afterRecvParticles(ParticleList& pl, class InBuffer &buf);
		virtual void onParticlesChanged();

		python::list getTriples();
        
        // get cos value for current triple
        //real getAngle(int, int, int);
        real getCos(int, int, int);

	    /** Get the number of triples in the GlobalTriples list */
	    int size() {
	    	return triplesCos.size();
	    }

		static void registerPython();
	

	  private:
		static LOG4ESPP_DECL_LOGGER(theLogger);
  };
}

#endif