#pragma once

class TransformMatrix 
  {
  public:
    enum TransformClass
      {
      IDENTITY,
      TRANSLATION,
      GENERAL,
      ORTHONORMAL,
      ORTHOGONAL,
      SCALE1D,
      SCALE3D,
      };

    TransformMatrix();
    void MakeIdentity();
    bool MakeScale(double i_factor);
    void  VerifyAndSetClass();
    bool IsIdentity() const;
    bool Transform3D(double* o_res,const double* i_res, bool i_vector) const;
    bool TransformPoint3D(double* o_res,const double* i_res) const;
    bool TransformVector3D(double* o_res,const double* i_res) const;
    bool MakeScale3D(double i_x,double i_y,double i_z);
    double* operator[](size_t);
    void GetTranslation(double* op_translation) const;
    void AddTranslation(const double* op_translation);
    bool MakeTranslation(const double* i_translation);
    TransformMatrix& PreMultiply(const TransformMatrix& i_arg);
    TransformMatrix& PostMultiply(const TransformMatrix& i_arg);
    bool Invert();

  private:
    void _VerifyAndSetHomogeneous();
    bool _VerifyAndSetIdentity();
    bool _VerifyAndSetTranslation();
    bool _VerifyAndSetScale1D();
    bool _VerifyAndSetScale3D();
    bool _VerifyAndSetOrthoNormal();
    bool _VerifyAndSetOrthoGonal();
    void _SetGeneral();

    bool _ComputeInverse();  // this calls one of the other protected methods, depending on the m_class field
    bool _ComputeInverseGeneral();
    bool _ComputeInverseIdentity();
    bool _ComputeInverseTranslation();
    bool _ComputeInverseOrthoNormal();
    bool _ComputeInverseOrthoGonal();
    bool _ComputeInverseScale1D();
    bool _ComputeInverseScale3D();

    void _InvertPivot(double o_matrix[4][8],size_t i_row); // protected for unittesting
    void _InvertDivideRowToOne(double o_matrix[4][8],size_t i_index); // protected for unittesting
    void _InvertSubtractRowFromRow(double o_matrix[4][8],size_t i_index_source,size_t i_index_destination); // protected for unittesting
    void _InvertSwapRows(double o_matrix[4][8],size_t i_index_one,size_t i_index_two);

    double m_matrix[4][4];
    double m_matrix_inverted[4][4];
    bool m_homogeneous;
    bool m_invertable;
    TransformClass m_class;
  };