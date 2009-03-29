static class MatrixUtils
{

  public static float[] deviceCoordsToWindowCoords(float[] deviceVec, int[] viewport)
  {
    return new float[]
    {
      viewport[0] + (((deviceVec[0] + 1) * (viewport[2])) / 2),
      viewport[1] + (((deviceVec[1] + 1) * (viewport[3])) / 2),
      (deviceVec[2] + 1) / 2
    };
  }

  public static float[] normalizeHomogeneousVector(float[] vec)
  {
    return new float[]
    {
      vec[0] / vec[3],
      vec[1] / vec[3],
      vec[2] / vec[3]
    };
  }

  public static int getMatrixIndex(int row, int col)
  {
    return col * 4 + row;
  }

  public static float[] multiplyMatrixByVector(final float[] mat, final float[] vec)
  {
    float[] product = new float[4];

    product[0] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2] + mat[3] * vec[3]; 
    product[1] = mat[4] * vec[0] + mat[5] * vec[1] + mat[6] * vec[2] + mat[7] * vec[3]; 
    product[2] = mat[8] * vec[0] + mat[9] * vec[1] + mat[10] * vec[2] + mat[11] * vec[3]; 
    product[3] = mat[12] * vec[0] + mat[13] * vec[1] + mat[14] * vec[2] + mat[15] * vec[3]; 

    return product;
  }

}

