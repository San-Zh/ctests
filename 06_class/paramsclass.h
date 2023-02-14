/**
 * @file base_drive.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <iostream>
#include <cstdio>

typedef struct _CG_params {
    int MaxIterNum;
    double TargetPrec;
} CGSolverParams;

using BICGSolverParams = CGSolverParams;

class SolverParams {
  private:
  public:
    double residual = 1.0e-9;
    SolverParams(){};
    virtual ~SolverParams(){};

  public:
    // virtual void setparams() = 0;
    // virtual void getparams() = 0;
};

class CGParams : public SolverParams {
  private:
  public:
    using SolverParams::residual;
    int MaxIter;
    CGParams() = default;
    ~CGParams() { printf("CGParams destroy\n"); }

  public:
    void setparams(int _MaxIter, double _rsd) { MaxIter = _MaxIter, residual = _rsd; }
    void getparams() { printf("residual = %lf\nMaxIter = %d\n", residual, MaxIter); }
};

class BiCGParams : public SolverParams {
  private:
  public:
    using SolverParams::residual;
    int MaxIter;
    BiCGParams() = default;
    ~BiCGParams() { printf("BiCGParams destroy\n"); }

  public:
    void setparams(int _MaxIter, double _rsd) { MaxIter = _MaxIter, residual = _rsd; }
    void getparams() { printf("residual = %lf\nMaxIter = %d\n", residual, MaxIter); }
};