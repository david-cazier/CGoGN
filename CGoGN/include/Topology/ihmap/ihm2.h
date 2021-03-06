/*******************************************************************************
* CGoGN: Combinatorial and Geometric modeling with Generic N-dimensional Maps  *
* version 0.1                                                                  *
* Copyright (C) 2009-2012, IGG Team, LSIIT, University of Strasbourg           *
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

#ifndef __IMPLICIT_HIERARCHICAL_MAP2__
#define __IMPLICIT_HIERARCHICAL_MAP2__

#include "Topology/map/embeddedMap2.h"

#include "Topology/dll.h"

namespace CGoGN
{

template<typename T, unsigned int ORBIT> class AttributeHandler_IHM ;


class CGoGN_TOPO_API ImplicitHierarchicalMap2 : public EmbeddedMap2
{
	template<typename T, unsigned int ORBIT> friend class AttributeHandler_IHM ;
	typedef EmbeddedMap2::TOPO_MAP TOPO_MAP;

private:
	unsigned int m_curLevel ;
	unsigned int m_maxLevel ;
	unsigned int m_idCount ;

	DartAttribute<unsigned int, ImplicitHierarchicalMap2> m_dartLevel ;
	DartAttribute<unsigned int, ImplicitHierarchicalMap2> m_edgeId ;

	AttributeMultiVector<unsigned int>* m_nextLevelCell[NB_ORBITS] ;

public:
	ImplicitHierarchicalMap2() ;

	~ImplicitHierarchicalMap2() ;

	//!
	/*!
	 *
	 */
	void update_topo_shortcuts();

	//!
	/*!
	 *
	 */
	void initImplicitProperties();

	/**
	 * clear the map
	 * @param remove attrib remove attribute (not only clear the content)
	 */
	void clear(bool removeAttrib);

	/***************************************************
	 *             ATTRIBUTES MANAGEMENT               *
	 ***************************************************/

	template <typename T, unsigned int ORBIT>
	AttributeHandler_IHM<T, ORBIT> addAttribute(const std::string& nameAttr) ;

	template <typename T, unsigned int ORBIT>
	AttributeHandler_IHM<T, ORBIT> getAttribute(const std::string& nameAttr) ;

	/***************************************************
	 *                 MAP TRAVERSAL                   *
	 ***************************************************/

	inline Dart newDart() ;

	inline Dart phi1(Dart d) const ;

	inline Dart phi_1(Dart d) const ;

	inline Dart phi2(Dart d) const ;

	inline Dart alpha0(Dart d) const ;

	inline Dart alpha1(Dart d) const ;

	inline Dart alpha_1(Dart d) const ;

	inline Dart begin() const ;

	inline Dart end() const ;

	inline void next(Dart& d) const ;

//	template <unsigned int ORBIT, typename FUNC>
//	void foreach_dart_of_orbit(Cell<ORBIT> c, FUNC f) const ;
	template <unsigned int ORBIT, typename FUNC>
	void foreach_dart_of_orbit(Cell<ORBIT> c, const FUNC& f) const ;

	template <typename FUNC>
	void foreach_dart_of_vertex(Dart d, FUNC& f) const;

	template <typename FUNC>
	void foreach_dart_of_edge(Dart d, FUNC& f) const ;

	template <typename FUNC>
	void foreach_dart_of_oriented_face(Dart d, FUNC& f) const ;

	template <typename FUNC>
	void foreach_dart_of_face(Dart d, FUNC& f)  const;

	template <typename FUNC>
	void foreach_dart_of_oriented_volume(Dart d, FUNC& f) const ;

	template <typename FUNC>
	void foreach_dart_of_volume(Dart d, FUNC& f) const ;

	template <typename FUNC>
	void foreach_dart_of_vertex1(Dart d, FUNC& f) const;

	template <typename FUNC>
	void foreach_dart_of_edge1(Dart d, FUNC& f) const;

	template <typename FUNC>
	void foreach_dart_of_cc(Dart d, FUNC& f) const ;

	/***************************************************
	 *               MAP MANIPULATION                  *
	 ***************************************************/

	void splitFace(Dart d, Dart e) ;

	unsigned int vertexDegree(Dart d);

	/***************************************************
	 *              LEVELS MANAGEMENT                  *
	 ***************************************************/

	unsigned int getCurrentLevel() ;

	void setCurrentLevel(unsigned int l) ;

	void incCurrentLevel();

	void decCurrentLevel();

	unsigned int getMaxLevel() ;

	unsigned int getDartLevel(Dart d) ;

	void setDartLevel(Dart d, unsigned int i) ;

	void setMaxLevel(unsigned int l);

	/***************************************************
	 *             EDGE ID MANAGEMENT                  *
	 ***************************************************/

	/**
	 * Give a new unique id to all the edges of the map
	 */
	void initEdgeId() ;

	/**
	 * Return the next available edge id
	 */
	unsigned int getNewEdgeId() ;

	unsigned int getEdgeId(Dart d) ;

	void setEdgeId(Dart d, unsigned int i) ;

	unsigned int getTriRefinementEdgeId(Dart d);

	unsigned int getQuadRefinementEdgeId(Dart d);

	/***************************************************
	 *               CELLS INFORMATION                 *
	 ***************************************************/

    /**
     * @brief faceDegree
     * @param d
     * @return
     */
    unsigned int faceDegree(Dart d);

	/**
	 * Return the level of insertion of the vertex of d
	 */
	unsigned int vertexInsertionLevel(Dart d) ;

//	/**
//	 * Return the level of the edge of d in the current level map
//	 */
//	unsigned int edgeLevel(Dart d) ;

//	/**
//	 * Return the level of the face of d in the current level map
//	 */
//	unsigned int faceLevel(Dart d) ;

//	/**
//	 * Given the face of d in the current level map,
//	 * return a level 0 dart of its origin face
//	 */
//	Dart faceOrigin(Dart d) ;

//	/**
//	 * Return the oldest dart of the face of d in the current level map
//	 */
//	Dart faceOldestDart(Dart d) ;

//	/**
//	 * Return true if the edge of d in the current level map
//	 * has already been subdivided to the next level
//	 */
//	bool edgeIsSubdivided(Dart d) ;

//	/**
//	 * Return true if the edge of d in the current level map
//	 * is subdivided to the next level,
//	 * none of its resulting edges is in turn subdivided to the next level
//	 * and the middle vertex is of degree 2
//	 */
//	bool edgeCanBeCoarsened(Dart d) ;

//	/**
//	 * Return true if the face of d in the current level map
//	 * has already been subdivided to the next level
//	 */
//	bool faceIsSubdivided(Dart d) ;

//	/**
//	 * Return true if the face of d in the current level map
//	 * is subdivided to the next level
//	 * and none of its resulting faces is in turn subdivided to the next level
//	 */
//	bool faceIsSubdividedOnce(Dart d) ;
} ;

template <typename T, unsigned int ORBIT>
class AttributeHandler_IHM : public AttributeHandler<T, ORBIT, ImplicitHierarchicalMap2>
{
public:
	typedef T DATA_TYPE ;

	AttributeHandler_IHM() : AttributeHandler<T, ORBIT, ImplicitHierarchicalMap2>()
	{}

	AttributeHandler_IHM(ImplicitHierarchicalMap2* m, AttributeMultiVector<T>* amv) : AttributeHandler<T, ORBIT, ImplicitHierarchicalMap2>(m, amv)
	{}

	AttributeMultiVector<T>* getDataVector() const
	{
		return AttributeHandler<T, ORBIT, ImplicitHierarchicalMap2>::getDataVector() ;
	}

	bool isValid() const
	{
		return AttributeHandler<T, ORBIT, ImplicitHierarchicalMap2>::isValid() ;
	}

	T& operator[](Dart d) ;

	const T& operator[](Dart d) const ;

	T& operator[](unsigned int a)
	{
		return AttributeHandler<T, ORBIT, ImplicitHierarchicalMap2>::operator[](a) ;
	}

	const T& operator[](unsigned int a) const
	{
		return AttributeHandler<T, ORBIT, ImplicitHierarchicalMap2>::operator[](a) ;
	}
} ;

template <typename T>
class VertexAttribute_IHM : public AttributeHandler_IHM<T, VERTEX>
{
public:
	VertexAttribute_IHM() : AttributeHandler_IHM<T, VERTEX>() {}
	VertexAttribute_IHM(const AttributeHandler_IHM<T, VERTEX>& ah) : AttributeHandler_IHM<T, VERTEX>(ah) {}
	VertexAttribute_IHM<T>& operator=(const AttributeHandler_IHM<T, VERTEX>& ah) { this->AttributeHandler_IHM<T, VERTEX>::operator=(ah); return *this; }
};


} //namespace CGoGN

#include "Topology/ihmap/ihm2.hpp"

#endif
