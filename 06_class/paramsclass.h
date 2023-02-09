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

class SolverParams {
  private:
  public:
    double residual = 1.0e-9;
    SolverParams() = default;
    virtual ~SolverParams() {}

  public:
    virtual void setparama() {}
    virtual void getparams() {}
};

class CGParams : public SolverParams {
  private:
  public:
    using SolverParams::residual;
    int MaxIter;
    CGParams() = default;
    ~CGParams() = default;

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
    ~BiCGParams() = default;

  public:
    void setparams(int _MaxIter, double _rsd) { MaxIter = _MaxIter, residual = _rsd; }
    void getparams() { printf("residual = %lf\nMaxIter = %d\n", residual, MaxIter); }
};