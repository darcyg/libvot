/*
Copyright (c) 2016, Tianwei Shen
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of libvot nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*
Copyright (c) 2007, 2008, 2011 libmv authors.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

/*! \file fundamental_matrix.h
 * \brief This is the solver of fundamental matrix.
 *
 */

#ifndef VOT_MULTIVIEW_FUNDAMENTAL_MATRIX_H
#define VOT_MULTIVIEW_FUNDAMENTAL_MATRIX_H

#include <vector>

#include "numeric/numeric.h"

namespace vot
{
namespace multiview
{
void ProjectionsFromFundamental(const Mat3 &F, Mat34 *P1, Mat34 *P2);
void FundamentalFromProjections(const Mat34 &P1, const Mat34 &P2, Mat3 *F);

/**
 * 7 points (minimal case, points coordinates must be normalized before):
 */
double FundamentalFrom7CorrespondencesLinear(const Mat &x1,
                                             const Mat &x2,
                                             std::vector<Mat3> *F);

/**
 * 7 points (points coordinates must be in image space):
 */
double FundamentalFromCorrespondences7Point(const Mat &x1,
                                            const Mat &x2,
                                            std::vector<Mat3> *F);

/**
 * 8 points (points coordinates must be in image space):
 */
double NormalizedEightPointSolver(const Mat &x1,
                                  const Mat &x2,
                                  Mat3 *F);

/**
 * Fundamental matrix utility function:
 */
void EnforceFundamentalRank2Constraint(Mat3 *F);

void NormalizeFundamental(const Mat3 &F, Mat3 *F_normalized);

/**
 * Approximate squared reprojection errror.
 *
 * See page 287 of HZ equation 11.9. This avoids triangulating the point,
 * relying only on the entries in F.
 */
double SampsonDistance(const Mat &F, const Vec2 &x1, const Vec2 &x2);

/**
 * Calculates the sum of the distances from the points to the epipolar lines.
 *
 * See page 288 of HZ equation 11.10.
 */
double SymmetricEpipolarDistance(const Mat &F, const Vec2 &x1, const Vec2 &x2);

/**
 * Compute the relative camera motion between two cameras.
 *
 * Given the motion parameters of two cameras, computes the motion parameters
 * of the second one assuming the first one to be at the origin.
 * If T1 and T2 are the camera motions, the computed relative motion is
 *   T = T2 T1^{-1}
 */
void RelativeCameraMotion(const Mat3 &R1,
                          const Vec3 &t1,
                          const Mat3 &R2,
                          const Vec3 &t2,
                          Mat3 *R,
                          Vec3 *t);

void EssentialFromFundamental(const Mat3 &F,
                              const Mat3 &K1,
                              const Mat3 &K2,
                              Mat3 *E);

void FundamentalFromEssential(const Mat3 &E,
                              const Mat3 &K1,
                              const Mat3 &K2,
                              Mat3 *F);

void EssentialFromRt(const Mat3 &R1,
                     const Vec3 &t1,
                     const Mat3 &R2,
                     const Vec3 &t2,
                     Mat3 *E);

void MotionFromEssential(const Mat3 &E,
                         std::vector<Mat3> *Rs,
                         std::vector<Vec3> *ts);

/**
 * Choose one of the four possible motion solutions from an essential matrix.
 *
 * Decides the right solution by checking that the triangulation of a match
 * x1--x2 lies in front of the cameras.  See HZ 9.6 pag 259 (9.6.3 Geometrical
 * interpretation of the 4 solutions)
 *
 * \return index of the right solution or -1 if no solution.
 */
int MotionFromEssentialChooseSolution(const std::vector<Mat3> &Rs,
                                      const std::vector<Vec3> &ts,
                                      const Mat3 &K1,
                                      const Vec2 &x1,
                                      const Mat3 &K2,
                                      const Vec2 &x2);

bool MotionFromEssentialAndCorrespondence(const Mat3 &E,
                                          const Mat3 &K1,
                                          const Vec2 &x1,
                                          const Mat3 &K2,
                                          const Vec2 &x2,
                                          Mat3 *R,
                                          Vec3 *t);

/**
 * Find closest essential matrix E to fundamental F
 */
void FundamentalToEssential(const Mat3 &F, Mat3 *E);

/**
 * This structure contains options that controls how the fundamental
 * estimation operates.
 *
 * Defaults should be suitable for a wide range of use cases, but
 * better performance and accuracy might require tweaking/
 */
struct EstimateFundamentalOptions {
	// Default constructor which sets up a options for generic usage.
	EstimateFundamentalOptions(void);

	// Maximal number of iterations for refinement step.
	int max_num_iterations;

	// Expected average of symmetric epipolar distance between
	// actual destination points and original ones transformed by
	// estimated fundamental matrix.
	//
	// Refinement will finish as soon as average of symmetric
	// epipolar distance is less or equal to this value.
	//
	// This distance is measured in the same units as input points are.
	double expected_average_symmetric_distance;
};

#ifdef LIBVOT_USE_CERES
///
/// \brief EstimateFundamentalFromCorrespondences: Fundamental transformation estimation.
/// This function estimates the fundamental transformation from a list of 2D
/// correspondences by doing algebraic estimation first followed with result refinement.
/// \param x1: points correspondences in the first view
/// \param x2: points correspondences in the second view
/// \param options: estimation options for fundamental matrix
/// \param F: result fundamental matrix
/// \return: true if success
///
bool EstimateFundamentalFromCorrespondences(const Mat &x1,
                                            const Mat &x2,
                                            const EstimateFundamentalOptions &options,
                                            Mat3 *F);
#endif
}	// end of namespace multiview
}	// end of namespace vot

#endif // VOT_MULTIVIEW_FUNDAMENTAL_MATRIX_H
