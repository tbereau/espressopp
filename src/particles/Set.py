import abc
from espresso.particles.Computer import *

class SetLocal(object):
    """Any derived object must define cxxobject, which must be a C++
    Set object. 
    """
    __metaclass__ = abc.ABCMeta
    def foreach(self, computer):
        """
        apply the computer to each particle in the set (i.e. call
        \"apply\" with the particle identity as
        parameter). 
        
        The return value of foreach is the return value of
        \"finalize\" on the master node.
        
        Example:

            >>> class MyPythonComputer(espresso.particles.PythonComputer) :
            >>>    def __init__(self) :
            >>>        self.count = 0
            >>>    def apply(self, id) :
            >>>        self.count += 1
            >>>    def collect(self) :
            >>>        return mpi.reduce(self.count, x,y : return x+y, pmi.CONTROLLER)
            >>>
            >>> decomposer.foreach(pmi.create(\"MyPythonComputer\"))
            """
        if isinstance(computer, PythonComputerLocal):
            cxxcomputer = computer
        else:
            cxxcomputer = computer.cxxobject
        self.cxxobject.foreach(cxxcomputer)

# TODO: implement:
#   * __contains__ from isMember
#   * __iter__ (via foreach?)

if pmi.IS_CONTROLLER:
    class Set(object):
        __metaclass__ = abc.ABCMeta
        def foreach(self, computer):
            if isinstance(computer, PythonComputerLocal):
                return pmi.call(self.pmiobject.foreach, computer)
            else:
                return pmi.call(self.pmiobject.foreach, computer.pmiobject)