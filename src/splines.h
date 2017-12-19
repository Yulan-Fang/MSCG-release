//
//  splines.h
//  
//
//  Copyright (c) 2016 The Voth Group at The University of Chicago. All rights reserved.
//

#ifndef _splines_h
#define _splines_h

#include <vector>
#include "gsl/gsl_bspline.h"

enum BasisType {kDelta = -1, kBSpline = 0, kLinearSpline = 1, kBSplineAndDeriv = 2, kNone = 3, kPower = 4, kInversePower = 5};

struct InteractionClassSpec;

class SplineComputer {

protected:
    unsigned n_coef;
    unsigned n_to_force_match;
    unsigned n_defined;
    double binwidth;

    InteractionClassSpec *ispec_;
    std::vector<unsigned> interaction_column_indices_;
    
public:
    SplineComputer(InteractionClassSpec* ispec);
    inline virtual ~SplineComputer() { }
    void get_bin(void);
    double get_param_less_lower_cutoff(const int index_among_defined, const double param_val) const;
    inline int get_n_coef(void) { return n_coef; };
    virtual void calculate_basis_fn_vals(const int index_among_defined, const double param_val, int &first_nonzero_basis_index, std::vector<double> &vals) = 0;
    virtual double evaluate_spline(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis) = 0;
};

SplineComputer* set_up_fm_spline_comp(InteractionClassSpec *ispec);
SplineComputer* set_up_table_spline_comp(InteractionClassSpec *ispec);

class BSplineComputer : public SplineComputer {  

protected:
    gsl_bspline_workspace** bspline_workspaces;
    gsl_vector* bspline_vectors;

public:
    BSplineComputer(InteractionClassSpec* ispec);
    virtual ~BSplineComputer();
    
   virtual void calculate_basis_fn_vals(const int index_among_defined, const double param_val, int &first_nonzero_basis_index, std::vector<double> &vals);
   virtual double evaluate_spline(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis);
};

class BSplineAndDerivComputer : public SplineComputer {

protected:
    int class_subtype;
    gsl_bspline_workspace** bspline_workspaces;
    gsl_vector* bspline_vectors;
    gsl_matrix* bspline_matrices;

public:
    BSplineAndDerivComputer(InteractionClassSpec* ispec);
    virtual ~BSplineAndDerivComputer();

   virtual void calculate_basis_fn_vals(const int index_among_defined, const double param_val, int &first_nonzero_basis_index, std::vector<double> &vals);
   void calculate_bspline_deriv_vals(const int index_among_defined, const double param_val, int &first_nonzero_basis_index, std::vector<double> &vals);
   virtual double evaluate_spline(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis);
   double evaluate_spline_deriv(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis); 
};

class LinearSplineComputer : public SplineComputer {

public:
    LinearSplineComputer(InteractionClassSpec* ispec);
    virtual ~LinearSplineComputer() {}

    virtual void calculate_basis_fn_vals(const int index_among_defined, const double param_val, int &first_nonzero_basis_index, std::vector<double> &vals);
    virtual double evaluate_spline(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis);
};

class DeltaSplineComputer : public SplineComputer {

public:
    DeltaSplineComputer(InteractionClassSpec* ispec);
    virtual ~DeltaSplineComputer() {}

    virtual void calculate_basis_fn_vals(const int index_among_defined, const double param_val, int &first_nonzero_basis_index, std::vector<double> &vals);
    virtual double evaluate_spline(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis);
};

class NoneSplineComputer : public SplineComputer {

public:
	NoneSplineComputer(InteractionClassSpec* ispec);
	virtual ~NoneSplineComputer() {}
	
    virtual void calculate_basis_fn_vals(const int index_among_defined, const double param_val, int &first_nonzero_basis_index, std::vector<double> &vals) {};
    virtual double evaluate_spline(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis) { return 0.0; };
};

class TableSplineComputer : public SplineComputer {

public:
    TableSplineComputer(InteractionClassSpec* ispec);
    virtual ~TableSplineComputer() {}

    virtual void calculate_basis_fn_vals(const int index_among_defined, const double param_val, int &first_nonzero_basis_index, std::vector<double> &vals);
    virtual double evaluate_spline(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis);
};


class PowerComputer : public SplineComputer {

 public:
  PowerComputer(InteractionClassSpec* ispec);
  virtual ~PowerComputer() {}

  virtual void calculate_basis_fn_vals(const int index_among_defined, const double param_vals, int &first_nonzero_basis_index, std::vector<double> &vals);
  void calculate_bspline_deriv_vals(const int index_among_defined, const double param_val, int &first_nonzero_basis_index, std::vector<double> &vals);
  virtual double evaluate_spline(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis);
  double evaluate_spline_deriv(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis); 
  virtual void power_eval(const double param_val, std::vector<double> &vals, unsigned n_coef);
  virtual void deriv_eval(const double param_val, std::vector<double> &vals, unsigned n_coef);
  virtual double power_axis(const int index_among_definedd, const std::vector<double> &spline_coeffs, const double axis_val);
  virtual double deriv_axis(const int index_among_definedd, const std::vector<double> &spline_coeffs, const double axis_val);
};

class InversePowerComputer : public SplineComputer {

 public:
  InversePowerComputer(InteractionClassSpec* ispec);
  virtual ~InversePowerComputer() {}

  virtual void calculate_basis_fn_vals(const int index_among_defined, const double param_vals, int &first_nonzero_basis_index, std::vector<double> &vals);
  void calculate_bspline_deriv_vals(const int index_among_defined, const double param_val, int &first_nonzero_basis_index, std::vector<double> &vals);
  virtual double evaluate_spline(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis);
  double evaluate_spline_deriv(const int index_among_defined, const int first_nonzero_basis_index, const std::vector<double> &spline_coeffs, const double axis); 
  virtual void power_eval(const double param_val, std::vector<double> &vals, unsigned n_coef);
  virtual void deriv_eval(const double param_val, std::vector<double> &vals, unsigned n_coef);
  virtual double power_axis(const int index_among_definedd, const std::vector<double> &spline_coeffs, const double axis_val);
  virtual double deriv_axis(const int index_among_definedd, const std::vector<double> &spline_coeffs, const double axis_val);

};
  

#endif
