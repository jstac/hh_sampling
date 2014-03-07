.. _hh_sampling:

******************************************************************
Perfect Simulation for Models of Industry Dynamics
******************************************************************

This page collects files and computer code for the paper **Perfect Simulation for Models of Industry Dynamics**
by Takashi Kamihigashi and John Stachurski.

Abstract
---------

In this paper we introduce a technique for perfect simulation from the stationary distribution of a standard model of industry dynamics. The method can be adapted to other, possibly non-monotone, regenerative processes found in industrial organization and other fields of economics.  The algorithm we propose is a version of coupling from the past. It is straightforward to implement and exploits the regenerative property of the process in order to achieve rapid coupling. 


Code
--------

An implementation is provided in C.  The only issue with portability is
that the C code uses the GSL library to generate the shocks.  However, any
other random number generator can be substituted.


