.. _hh_sampling:

******************************************************************
Exact Sampling from the Stationary Distribution of Entry-Exit Models
******************************************************************

This page collects files and computer code for the paper **Exact Sampling from
the Stationary Distribution of Entry-Exit Models** by Takashi Kamihigashi
and John Stachurski.

Abstract
---------

In equilibrium models of firm dynamics, the stationary equilibrium
distribution of firms summarizes the predictions of the model for a given set
of primitives.  Focusing on Hopenhayn's seminal model of firm dynamics with
entry and exit (Econometrica, 60:5, 1992, p.~1127--1150), we provide an
algorithm that samples *exactly* from the stationary distribution for any
specified exit threshold.   The algorithm is able to rapidly generate large
numbers of exact and independent draws from the stationary distribution, and
thus can be used to obtain unbiased estimates and confidence intervals for
moments and distributions of interest.


Code
--------

An implementation is provided in C.  The only issue with portability is
that the C code uses the GSL library to generate the shocks.  However, any
other random number generator can be substituted.


