#include "TransformMatrix.h"
#include <math.h>
#include <utility>

#define DBL_1D_EPSILON 2.2204460492503131e-016

class MMTransformException
  {
  public:
    // constants
    ////////////
    enum Kinds
      {
      HOMOGENEOUS_ZERO,
      NOT_YET_IMPLEMENTED,
      MULTIPLY_BASE_CLASS,
      NOT_INVERSIBLE,
      TRANSFORM_NOT_POSSIBLE,
      WRONG_CLASS,
      CANNOT_CREATE_TRANSFORM,
      CANNOT_CONCATTENATE
      };
    // Member functions
    ///////////////////
    MMTransformException(Kinds i_kind):
      m_kind(i_kind)
      {

      }

    // Data Members
    ///////////////
    Kinds m_kind;
  };

//--------------------------------------------------------------------------------------------------------------------------------------------------------
TransformMatrix::TransformMatrix()
  {
  MakeIdentity();
  }

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void TransformMatrix::MakeIdentity()
  {
  m_matrix[0][0]=1;m_matrix[0][1]=0;m_matrix[0][2]=0;m_matrix[0][3]=0;
  m_matrix[1][0]=0;m_matrix[1][1]=1;m_matrix[1][2]=0;m_matrix[1][3]=0;
  m_matrix[2][0]=0;m_matrix[2][1]=0;m_matrix[2][2]=1;m_matrix[2][3]=0;
  m_matrix[3][0]=0;m_matrix[3][1]=0;m_matrix[3][2]=0;m_matrix[3][3]=1;

  m_matrix_inverted[0][0]=1;m_matrix_inverted[0][1]=0;m_matrix_inverted[0][2]=0;m_matrix_inverted[0][3]=0;
  m_matrix_inverted[1][0]=0;m_matrix_inverted[1][1]=1;m_matrix_inverted[1][2]=0;m_matrix_inverted[1][3]=0;
  m_matrix_inverted[2][0]=0;m_matrix_inverted[2][1]=0;m_matrix_inverted[2][2]=1;m_matrix_inverted[2][3]=0;
  m_matrix_inverted[3][0]=0;m_matrix_inverted[3][1]=0;m_matrix_inverted[3][2]=0;m_matrix_inverted[3][3]=1;

  m_homogeneous = false;
  m_invertable=true;
  m_class=IDENTITY;
  };

//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool TransformMatrix::MakeScale(double i_factor)
  {
  for (size_t i=0;i<4;++i)
    for (size_t j=0;j<4;++j)
      m_matrix[i][j]= i==j ? i_factor : 0;

  m_matrix[3][3]=1;

  VerifyAndSetClass();
  m_invertable=_ComputeInverse();

  return true;
  };

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void  TransformMatrix::VerifyAndSetClass()
  {
  _VerifyAndSetHomogeneous();

  if (!_VerifyAndSetIdentity())
    if (!_VerifyAndSetTranslation())
      if (!_VerifyAndSetScale1D())
        if (!_VerifyAndSetScale3D())
          if (!_VerifyAndSetOrthoNormal())
            if (!_VerifyAndSetOrthoGonal())
              _SetGeneral();
  }

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void TransformMatrix::_VerifyAndSetHomogeneous()
  {
  if (m_matrix[3][0]==0 && m_matrix[3][1]==0 && m_matrix[3][2]==0 && m_matrix[3][3]==1)
    m_homogeneous = false;
  else
    m_homogeneous = true;
  };

//-----------------------------------------------------------------------------
bool TransformMatrix::_VerifyAndSetIdentity()
  {
  bool success=true;

  for (size_t i=0;i<4;++i)
    for (size_t j=0;j<4;++j)  
      {
      success=success && (m_matrix[i][j]== (i==j ? 1 : 0) );
      }

    if (success)
      {
      m_class=IDENTITY;
      return true;
      }
    else
      {
      return false;    
      }
  };

//-----------------------------------------------------------------------------
bool TransformMatrix::_VerifyAndSetTranslation()
  {
  bool success=true;

  for (size_t i=0;i<4;++i)
    for (size_t j=0;j<4;++j)  
      {
      if (j!=3)
        success = success && (abs(m_matrix[i][j] - (i==j ? 1 : 0)) < 10*DBL_1D_EPSILON);
      else if (i==3)
        success = success && (abs(m_matrix[i][j] - 1) < 10*DBL_1D_EPSILON);
      }

    if (success)
      {
      m_class=TRANSLATION;
      return true;
      }
    else
      {
      return false;    
      }
  };

//-----------------------------------------------------------------------------
bool TransformMatrix::_VerifyAndSetScale1D()
  {
  bool success=true;
  double factor=m_matrix[0][0];

  for (size_t i=0;i<4;++i)
    for (size_t j=0;j<4;++j)  
      {
      if (i==j && i!=3)
        success = success && m_matrix[i][j]==factor;
      else if (i==3 && j==3)
        success = success && m_matrix[i][j]==1;
      else
        success = success && m_matrix[i][j]==0;
      }

    if (success)
      {
      m_class=SCALE1D;
      return true;
      }
    else
      {
      return false;    
      }

  };

//-----------------------------------------------------------------------------
bool TransformMatrix::_VerifyAndSetScale3D()
  {
  bool success=true;

  for (size_t i=0;i<4;++i)
    for (size_t j=0;j<4;++j)  
      {
      if (i!=j)
        success = success && m_matrix[i][j]==0;
      }

    success = success && m_matrix[3][3]==1;

    if (success)
      {
      m_class=SCALE3D;
      return true;
      }
    else
      {
      return false;    
      }
  };

//-----------------------------------------------------------------------------
bool TransformMatrix::_VerifyAndSetOrthoNormal()
  {
  bool success=true;    
  // check unit vectors
  success= success && abs(m_matrix[0][0]*m_matrix[0][0]+m_matrix[1][0]*m_matrix[1][0]+m_matrix[2][0]*m_matrix[2][0]-1) < 10*DBL_1D_EPSILON;
  success= success && abs(m_matrix[0][1]*m_matrix[0][1]+m_matrix[1][1]*m_matrix[1][1]+m_matrix[2][1]*m_matrix[2][1]-1) < 10*DBL_1D_EPSILON;
  success= success && abs(m_matrix[0][2]*m_matrix[0][2]+m_matrix[1][2]*m_matrix[1][2]+m_matrix[2][2]*m_matrix[2][2]-1) < 10*DBL_1D_EPSILON;
  // check perpendicular
  success= success && abs(m_matrix[0][0]*m_matrix[1][0]+m_matrix[0][1]*m_matrix[1][1]+m_matrix[0][2]*m_matrix[1][2]) < 10*DBL_1D_EPSILON;
  success= success && abs(m_matrix[0][0]*m_matrix[2][0]+m_matrix[0][1]*m_matrix[2][1]+m_matrix[0][2]*m_matrix[2][2]) < 10*DBL_1D_EPSILON;
  success= success && abs(m_matrix[1][0]*m_matrix[2][0]+m_matrix[1][1]*m_matrix[2][1]+m_matrix[1][2]*m_matrix[2][2]) < 10*DBL_1D_EPSILON;

  for (size_t i=0;i<3;++i)
    {
    success=success && m_matrix[3][i]==0;
    }
  success=success && m_matrix[3][3]==1;


  if (success)
    {
    m_class=ORTHONORMAL;
    return true;
    }
  else
    {
    return false;
    }
  };

//-----------------------------------------------------------------------------
bool TransformMatrix::_VerifyAndSetOrthoGonal()
  {
  bool success=true;    
  // check unit vectors
  double factor = m_matrix[0][0]*m_matrix[0][0]+m_matrix[1][0]*m_matrix[1][0]+m_matrix[2][0]*m_matrix[2][0];
  success= success && abs(m_matrix[0][1]*m_matrix[0][1]+m_matrix[1][1]*m_matrix[1][1]+m_matrix[2][1]*m_matrix[2][1]-factor) < DBL_1D_EPSILON;
  success= success && abs(m_matrix[0][2]*m_matrix[0][2]+m_matrix[1][2]*m_matrix[1][2]+m_matrix[2][2]*m_matrix[2][2]-factor) < DBL_1D_EPSILON;
  // check perpendicular
  success= success && abs(m_matrix[0][0]*m_matrix[1][0]+m_matrix[0][1]*m_matrix[1][1]+m_matrix[0][2]*m_matrix[1][2]) < DBL_1D_EPSILON;
  success= success && abs(m_matrix[0][0]*m_matrix[2][0]+m_matrix[0][1]*m_matrix[2][1]+m_matrix[0][2]*m_matrix[2][2]) < DBL_1D_EPSILON;
  success= success && abs(m_matrix[1][0]*m_matrix[2][0]+m_matrix[1][1]*m_matrix[2][1]+m_matrix[1][2]*m_matrix[2][2]) < DBL_1D_EPSILON;

  for (size_t i=0;i<3;++i)
    {
    success=success && m_matrix[i][3]==0;
    success=success && m_matrix[3][i]==0;
    }
  success=success && m_matrix[3][3]==1;

  if (success)
    {
    m_class=ORTHOGONAL;
    return true;
    }
  else
    {
    return false;
    }

  };

//-----------------------------------------------------------------------------
void TransformMatrix::_SetGeneral()
  {
  m_class=GENERAL;
  };

//-----------------------------------------------------------------------------
bool TransformMatrix::_ComputeInverse()
  {
  switch (m_class)
    {
  case GENERAL:
    return m_invertable=_ComputeInverseGeneral();
  case IDENTITY:    
    return m_invertable=_ComputeInverseIdentity();
  case TRANSLATION:
    return m_invertable=_ComputeInverseTranslation();
  case ORTHONORMAL:
    return m_invertable=_ComputeInverseOrthoNormal();
  case ORTHOGONAL:
    return m_invertable=_ComputeInverseOrthoGonal();
  case SCALE1D :
    return m_invertable=_ComputeInverseScale1D();
  case SCALE3D :
    return m_invertable=_ComputeInverseScale3D();
  default :
    return false;
    }
  }

//------------------------------------------------------------------------------------
bool TransformMatrix::_ComputeInverseGeneral()
  {
  try
    {
    double temp[4][8];
      {
      for (size_t i=0;i<4;++i)
        for (size_t j=0;j<4;++j)
          {
          temp[i][j]=m_matrix[i][j];
          temp[i][j+4]= i==j ? 1 : 0;
          }
      }
      // do a gaussian elimination to obtain an identity in the left square
      {
      for (size_t i=0;i<4;++i)
        {
        _InvertPivot(temp,i);
        _InvertDivideRowToOne(temp,i);
        for (size_t j=0;j<4;++j)
          {
          if (j!=i)
            {
            _InvertSubtractRowFromRow(temp,i,j);
            }
          }
        }
      }
      // the right square is now miraculously the inverse
      {
      for (size_t i=0;i<4;++i)
        for (size_t j=0;j<4;++j)
          {
          m_matrix_inverted[i][j]=temp[i][j+4];
          }
      }
    }
  catch (MMTransformException)
    {
    return false;
    }
  return true;
  };
  
  
  //-----------------------------------------------------------------------------
bool
  TransformMatrix::_ComputeInverseOrthoNormal()
  {
  // transpose
  m_matrix_inverted[0][0]=m_matrix[0][0];
  m_matrix_inverted[1][0]=m_matrix[0][1];
  m_matrix_inverted[2][0]=m_matrix[0][2];

  m_matrix_inverted[0][1]=m_matrix[1][0];
  m_matrix_inverted[1][1]=m_matrix[1][1];
  m_matrix_inverted[2][1]=m_matrix[1][2];

  m_matrix_inverted[0][2]=m_matrix[2][0];
  m_matrix_inverted[1][2]=m_matrix[2][1];
  m_matrix_inverted[2][2]=m_matrix[2][2];

  // just to keep it clean
  m_matrix_inverted[3][0]=0;
  m_matrix_inverted[3][1]=0;
  m_matrix_inverted[3][2]=0;
  m_matrix_inverted[3][3]=1;

  // translation
  m_matrix_inverted[0][3]=-(m_matrix[0][0]*m_matrix[0][3]+m_matrix[1][0]*m_matrix[1][3]+m_matrix[2][0]*m_matrix[2][3]);
  m_matrix_inverted[1][3]=-(m_matrix[0][1]*m_matrix[0][3]+m_matrix[1][1]*m_matrix[1][3]+m_matrix[2][1]*m_matrix[2][3]);
  m_matrix_inverted[2][3]=-(m_matrix[0][2]*m_matrix[0][3]+m_matrix[1][2]*m_matrix[1][3]+m_matrix[2][2]*m_matrix[2][3]);

  return true; // never seems to go wrong.
  };

//-----------------------------------------------------------------------------
bool 
  TransformMatrix::_ComputeInverseIdentity()
  {
  for (size_t i=0;i<4;++i)
    for (size_t j=0;j<4;++j)
      m_matrix_inverted[i][j] = i==j ? 1 : 0;
  return true;  // usually works
  };
  
  //-----------------------------------------------------------------------------
bool TransformMatrix::_ComputeInverseOrthoGonal()
  {
  // compute a length squared
  double factor = m_matrix[0][0]*m_matrix[0][0]+m_matrix[1][0]*m_matrix[1][0]+m_matrix[2][0]*m_matrix[2][0];
  if (factor==0)
    return false;
  // transpose
  m_matrix_inverted[0][0]=m_matrix[0][0]/factor;
  m_matrix_inverted[1][0]=m_matrix[0][1]/factor;
  m_matrix_inverted[2][0]=m_matrix[0][2]/factor;

  m_matrix_inverted[0][1]=m_matrix[1][0]/factor;
  m_matrix_inverted[1][1]=m_matrix[1][1]/factor;
  m_matrix_inverted[2][1]=m_matrix[1][2]/factor;

  m_matrix_inverted[0][2]=m_matrix[2][0]/factor;
  m_matrix_inverted[1][2]=m_matrix[2][1]/factor;
  m_matrix_inverted[2][2]=m_matrix[2][2]/factor;

  // just to keep it clean
  m_matrix_inverted[3][0]=0;
  m_matrix_inverted[3][1]=0;
  m_matrix_inverted[3][2]=0;
  m_matrix_inverted[3][3]=1;

  // translation
  m_matrix_inverted[0][3]=-(m_matrix[0][0]*m_matrix[0][3]+m_matrix[1][0]*m_matrix[1][3]+m_matrix[2][0]*m_matrix[2][3]);
  m_matrix_inverted[1][3]=-(m_matrix[0][1]*m_matrix[0][3]+m_matrix[1][1]*m_matrix[1][3]+m_matrix[2][1]*m_matrix[2][3]);
  m_matrix_inverted[2][3]=-(m_matrix[0][2]*m_matrix[0][3]+m_matrix[1][2]*m_matrix[1][3]+m_matrix[2][2]*m_matrix[2][3]);

  return true;

  };
  
  //-----------------------------------------------------------------------------
bool TransformMatrix::_ComputeInverseScale1D()
  {
  double factor=m_matrix[0][0];
  if (factor==0)
    {
    return false;
    }
  factor=1/factor;
  for (size_t i=0;i<3;++i)
    for (size_t j=0;j<3;++j)
      m_matrix_inverted[i][j]= i==j ? factor : 0;

  m_matrix_inverted[3][3]=1;


  return true;
  };

  //-----------------------------------------------------------------------------
bool TransformMatrix::_ComputeInverseScale3D()
  {
  for (size_t i=0;i<4;++i)
    for (size_t j=0;j<4;++j)
      {
      if (i==j)
        {
        if (m_matrix[i][j]==0)
          return false;
        m_matrix_inverted[i][j]=1/m_matrix[i][j];
        }
      else
        {
        m_matrix_inverted[i][j]=0;
        }

      }
    m_matrix_inverted[3][3]=1;

    return true;
  };

//-----------------------------------------------------------------------------
bool TransformMatrix::_ComputeInverseTranslation()
  {
  for (size_t i=0;i<4;++i)
    for (size_t j=0;j<3;++j)
      m_matrix_inverted[i][j] = (i == j) ? 1 : 0;

  m_matrix_inverted[0][3]=-m_matrix[0][3];
  m_matrix_inverted[1][3]=-m_matrix[1][3];
  m_matrix_inverted[2][3]=-m_matrix[2][3];
  m_matrix_inverted[3][3]=1;

  return true;
  };
//-----------------------------------------------------------------------------
void
  TransformMatrix::_InvertDivideRowToOne(double o_matrix[4][8],size_t i_index)
  {
  ASSERT(i_index<4);
  double divisor=o_matrix[i_index][i_index];
  if (divisor==0)
    throw MMTransformException(MMTransformException::NOT_INVERSIBLE);
  o_matrix[i_index][i_index]=1;
  for (size_t i=i_index+1;i<8;++i)
    {
    double res=o_matrix[i_index][i]/divisor;
    o_matrix[i_index][i]=res;
    }
  }

void TransformMatrix::_InvertSwapRows(double o_matrix[4][8],size_t i_index_one,size_t i_index_two)
  {
  if (i_index_one!=i_index_two)
    {
    for (size_t i=0;i<8;++i)
      {
      std::swap(o_matrix[i_index_one][i],o_matrix[i_index_two][i]);
      }
    }
  }

void TransformMatrix::_InvertPivot(double o_matrix[4][8],size_t i_row)
  {
  size_t max_index=i_row;
  double max=abs(o_matrix[i_row][i_row]);
  for (size_t i=i_row+1;i<4;++i)
    {
    if (max<abs(o_matrix[i][i_row]))
      {
      max=abs(o_matrix[i][i_row]);
      max_index=i;
      }
    }
  _InvertSwapRows(o_matrix,i_row,max_index);
  }


void TransformMatrix::_InvertSubtractRowFromRow(double o_matrix[4][8],size_t i_index_source,size_t i_index_destination)
  {
  ASSERT(o_matrix[i_index_source][i_index_source]==1);  
  double factor=o_matrix[i_index_destination][i_index_source];
  o_matrix[i_index_destination][i_index_source]=0;
  for (size_t i=i_index_source+1;i<8;++i)
    o_matrix[i_index_destination][i]-=o_matrix[i_index_source][i]*factor;
  }


//-----------------------------------------------------------------------------
bool TransformMatrix::IsIdentity() const
  {
  return m_class==IDENTITY;
  };

//-----------------------------------------------------------------------------
bool TransformMatrix::TransformVector3D(double* o_res, const double* i_res) const
  {
  return Transform3D(o_res, i_res, true);
  }

//-----------------------------------------------------------------------------
bool TransformMatrix::TransformPoint3D(double* o_res, const double* i_res) const
  {
  return Transform3D(o_res, i_res, false);
  }

//-----------------------------------------------------------------------------
bool TransformMatrix::Transform3D(double* o_res,const double* i_res, bool i_vector) const
  {
  if( IsIdentity() )
    {
    memcpy(o_res, i_res, sizeof(double)*3);
    return true;
    }

  double point[3]={i_res[0],i_res[1],i_res[2]};  
  double result[4];  
  if (i_vector)
    {
    result[0]=m_matrix[0][0]*point[0]+m_matrix[0][1]*point[1]+m_matrix[0][2]*point[2];
    result[1]=m_matrix[1][0]*point[0]+m_matrix[1][1]*point[1]+m_matrix[1][2]*point[2];
    result[2]=m_matrix[2][0]*point[0]+m_matrix[2][1]*point[1]+m_matrix[2][2]*point[2];
    if (m_homogeneous)
      result[3]=m_matrix[3][0]*point[0]+m_matrix[3][1]*point[1]+m_matrix[3][2]*point[2];
    }
  else
    {  
    result[0]=m_matrix[0][0]*point[0]+m_matrix[0][1]*point[1]+m_matrix[0][2]*point[2]+m_matrix[0][3];
    result[1]=m_matrix[1][0]*point[0]+m_matrix[1][1]*point[1]+m_matrix[1][2]*point[2]+m_matrix[1][3];
    result[2]=m_matrix[2][0]*point[0]+m_matrix[2][1]*point[1]+m_matrix[2][2]*point[2]+m_matrix[2][3];
    if (m_homogeneous)
      result[3]=m_matrix[3][0]*point[0]+m_matrix[3][1]*point[1]+m_matrix[3][2]*point[2]+m_matrix[3][3];
    }

  if (m_homogeneous)
    {
    if (result[3]==0)
      {
      throw MMTransformException(MMTransformException::HOMOGENEOUS_ZERO);
      }
    o_res[0]=result[0]/result[3];  
    o_res[1]=result[1]/result[3];  
    o_res[2]=result[2]/result[3];  
    }
  else
    {
    o_res[0]=result[0];  
    o_res[1]=result[1];  
    o_res[2]=result[2];      
    }
  return true;
  };

//-----------------------------------------------------------------------------
bool TransformMatrix::MakeScale3D(double i_x,double i_y,double i_z)
  {
  for (size_t i=0;i<4;++i)
    for (size_t j=0;j<4;++j)
      m_matrix[i][j]= 0;

  m_matrix[0][0]=i_x;
  m_matrix[1][1]=i_y;
  m_matrix[2][2]=i_z;
  m_matrix[3][3]=1;

  VerifyAndSetClass();
  m_invertable=_ComputeInverse();

  return true;

  };

//-----------------------------------------------------------------------------
double* TransformMatrix::operator[]( size_t i)
  {
  return &m_matrix[i][0];
  }

//-----------------------------------------------------------------------------
void TransformMatrix::GetTranslation( double* op_translation ) const
  {
  op_translation[0]=m_matrix[0][3];
  op_translation[1]=m_matrix[1][3];
  op_translation[2]=m_matrix[2][3];
  }

//-----------------------------------------------------------------------------
void TransformMatrix::AddTranslation(const double* i_translation)
  {
  TransformMatrix translation_matrix;
  translation_matrix.MakeTranslation(i_translation);
  PreMultiply(translation_matrix);

  VerifyAndSetClass();  
  }

//-----------------------------------------------------------------------------
TransformMatrix& TransformMatrix::PreMultiply(const TransformMatrix& i_arg)
  {
  double temp[4][4];
    {
    for (size_t i=0;i<4;++i)
      for (size_t j=0;j<4;++j)
        {
        temp[i][j]=i_arg.m_matrix[i][0]*m_matrix[0][j]+i_arg.m_matrix[i][1]*m_matrix[1][j]+i_arg.m_matrix[i][2]*m_matrix[2][j]+i_arg.m_matrix[i][3]*m_matrix[3][j];
        }
    }
    {
    for (size_t i=0;i<4;++i)
      for (size_t j=0;j<4;++j)
        {
        m_matrix[i][j]=temp[i][j];
        }      
    }
    {
    for (size_t i=0;i<4;++i)
      for (size_t j=0;j<4;++j)
        {
        temp[i][j]=m_matrix_inverted[i][0]*i_arg.m_matrix_inverted[0][j]+m_matrix_inverted[i][1]*i_arg.m_matrix_inverted[1][j]+m_matrix_inverted[i][2]*i_arg.m_matrix_inverted[2][j]+m_matrix_inverted[i][3]*i_arg.m_matrix_inverted[3][j];
        }
    }
    {
    for (size_t i=0;i<4;++i)
      for (size_t j=0;j<4;++j)
        {
        m_matrix_inverted[i][j]=temp[i][j];
        }
    }

    VerifyAndSetClass();
    return *this;  
  };

//-----------------------------------------------------------------------------
bool TransformMatrix::MakeTranslation(const double* i_translation)
  {
  m_matrix[0][3] = i_translation[0];
  m_matrix[1][3] = i_translation[1];
  m_matrix[2][3] = i_translation[2];

  m_matrix[0][0] = 1; m_matrix[0][1] = 0; m_matrix[0][2] = 0;
  m_matrix[1][0] = 0; m_matrix[1][1] = 1; m_matrix[1][2] = 0;
  m_matrix[2][0] = 0; m_matrix[2][1] = 0; m_matrix[2][2] = 1;

  m_matrix[3][0]=0;
  m_matrix[3][1]=0;
  m_matrix[3][2]=0;
  m_matrix[3][3]=1;

  VerifyAndSetClass();
  m_invertable=_ComputeInverse();

  return true;
  };

//-----------------------------------------------------------------------------
TransformMatrix& TransformMatrix::PostMultiply(const TransformMatrix& i_arg)
  {
  double temp[4][4];
    {
    for (size_t i=0;i<4;++i)
      for (size_t j=0;j<4;++j)
        {
        temp[i][j]=m_matrix[i][0]*i_arg.m_matrix[0][j]+m_matrix[i][1]*i_arg.m_matrix[1][j]+m_matrix[i][2]*i_arg.m_matrix[2][j]+m_matrix[i][3]*i_arg.m_matrix[3][j];
        }
    }
    {
    for (size_t i=0;i<4;++i)
      for (size_t j=0;j<4;++j)
        {
        m_matrix[i][j]=temp[i][j];
        }      
    }
    {
    for (size_t i=0;i<4;++i)
      for (size_t j=0;j<4;++j)
        {
        temp[i][j]=i_arg.m_matrix_inverted[i][0]*m_matrix_inverted[0][j]+i_arg.m_matrix_inverted[i][1]*m_matrix_inverted[1][j]+i_arg.m_matrix_inverted[i][2]*m_matrix_inverted[2][j]+i_arg.m_matrix_inverted[i][3]*m_matrix_inverted[3][j];
        }
    }
    {
    for (size_t i=0;i<4;++i)
      for (size_t j=0;j<4;++j)
        {
        m_matrix_inverted[i][j]=temp[i][j];
        }
    }

  VerifyAndSetClass();
  return *this;
  };

//--------------------------------------------------------------------------------------------
bool TransformMatrix::Invert()
  {
  if (!m_invertable)
    return false;
  for (size_t i=0;i<4;++i)
    for (size_t j=0;j<4;++j)
      {
      std::swap(m_matrix[i][j],m_matrix_inverted[i][j]);
      }

  return true;
  }
