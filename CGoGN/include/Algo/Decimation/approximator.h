/*******************************************************************************
* CGoGN: Combinatorial and Geometric modeling with Generic N-dimensional Maps  *
* version 0.1                                                                  *
* Copyright (C) 2009-2013, IGG Team, ICube, University of Strasbourg           *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Web site: http://cgogn.unistra.fr/                                           *
* Contact information: cgogn@unistra.fr                                        *
*                                                                              *
*******************************************************************************/

#include "Algo/Decimation/predictor.h"

#ifndef __APPROXIMATOR_H__
#define __APPROXIMATOR_H__

namespace CGoGN
{

namespace Algo
{

namespace Surface
{

namespace Decimation
{

enum ApproximatorType
{
	// One approx per edge
	// Geometry approximators
	A_QEM = 0, /**< Approximates the geometry of an edge collapse by quadric error metric minimization [GH97]. */
	A_MidEdge = 1, /**< Approximates the geometry of an edge collapse by placing the resulting vertex in its middle. */
	A_CornerCutting = 2,
	A_TangentPredict1 = 3,
	A_TangentPredict2 = 4,
	A_NormalArea = 5, /**< EXPERIMENTAL Approximates the geometry of an edge collapse by minimization of its normal times area measure [Sauvage] */
	// Geometry + color approximators
	A_ColorNaive = 6, /**< Approximates the color of the resulting vertex by linear interpolation (based on the approximated position) of its two predecessors. */
	A_ColorQEMext = 7, /**< Approximates both geometry and color of the resulting vertex by minimization of the extended (R^6) quadric error metric [GH98]. */
	A_GeomColorOpt = 8, /**< EXPERIMENTAL. */

	// One approx per half-edge
	// Generic (considers all provided attributes) approximator
	A_hHalfCollapse = 9, /**< Approximates all provided attributes of a half-edge collapse by keeping the attributes of the first of two vertices. */
	// Geometry approximator
	A_hQEM = 10, /**< Approximates the geometry of a full-edge collapse by quadric error metric minimization [GH97]. Compatible version for half-edge selectors. */

	A_OTHER /**< Can be used for extensions. */
} ;

/*!
 * \class ApproximatorGen
 * \brief Generic class holder for approximators
 */
template <typename PFP>
class ApproximatorGen
{
protected:
	typename PFP::MAP& m_map ;

public:
	ApproximatorGen(typename PFP::MAP& m) : m_map(m)
	{}
	virtual ~ApproximatorGen()
	{}
	virtual const std::string& getApproximatedAttributeName() const = 0 ;
	virtual ApproximatorType getType() const = 0 ;

	virtual bool init() = 0 ;
	virtual void approximate(Dart d) = 0 ;

	virtual void saveApprox(Dart d) = 0 ;
	virtual void affectApprox(Dart d) = 0 ;

	virtual const PredictorGen<PFP>* getPredictor() const = 0 ;
//	virtual REAL detailMagnitude(Dart d) = 0 ;
//	virtual void addDetail(Dart d, double amount, bool sign, typename PFP::MATRIX33* detailTransform) = 0 ;
} ;

/*!
 * \class Approximator
 * \brief Generic class for approximators
 */
template <typename PFP, typename T, unsigned int ORBIT>
class Approximator : public ApproximatorGen<PFP>
{
	typedef typename PFP::MAP MAP ;

protected:
	VertexAttribute<T, MAP>& m_attr ;	// vertex attribute to be approximated
	AttributeHandler<T, ORBIT, MAP> m_approx ;	// attribute to store approximation result
	AttributeHandler<T, ORBIT, MAP> m_detail ;	// attribute to store detail information for reconstruction
	T m_app ;

	Predictor<PFP, T>* m_predictor ;

public:
	Approximator(MAP& m, VertexAttribute<T, MAP>& attr, Predictor<PFP, T>* predictor) ;
	virtual ~Approximator() ;

	const VertexAttribute<T, MAP>& getApproximatedAttribute() const ;
	VertexAttribute<T, MAP>& getApproximatedAttribute() ;
	const std::string& getApproximatedAttributeName() const ;

	const AttributeHandler<T, ORBIT, MAP>& getApproximationResultAttribute() const;

	void saveApprox(Dart d) ;
	void affectApprox(Dart d) ;
	const T& getApprox(Dart d) const ;

	const Predictor<PFP, T>* getPredictor() const ;
	const T& getDetail(Dart d) const ;
	void setDetail(Dart d, const T& val) ;
//	REAL detailMagnitude(Dart d) ; // TODO works only for vector types !!
//	void addDetail(Dart d, double amount, bool sign, typename PFP::MATRIX33* detailTransform) ; // TODO works only for vector types !!
} ;

} // namespace Decimation

} // namespace surface

} // namespace Algo

} // namespace CGoGN

#include "Algo/Decimation/approximator.hpp"

#endif
