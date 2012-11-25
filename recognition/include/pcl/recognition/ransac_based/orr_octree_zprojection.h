/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2010-2012, Willow Garage, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

/*
 * orr_octree_zprojection.h
 *
 *  Created on: Nov 17, 2012
 *      Author: papazov
 */

#ifndef ORR_OCTREE_ZPROJECTION_H_
#define ORR_OCTREE_ZPROJECTION_H_

#include "orr_octree.h"
#include <pcl/pcl_exports.h>
#include <set>

class ORROctree;

namespace pcl
{
  namespace recognition
  {
    class PCL_EXPORTS ORROctreeZProjection
    {
      public:
        class Pixel
        {
          public:
            inline void
            set_z1 (float z1) { z1_ = z1;}

            inline void
            set_z2 (float z2) { z2_ = z2;}

          public:
            float z1_, z2_;
            /** \brief These are the ids of the hypotheses at this pixel. */
            std::list<int> hypotheses_ids_;
        };

      public:
        class Set
        {
          public:
        	Set (int x, int y)
        	: nodes_ (compare_nodes_z), x_ (x), y_ (y)
        	{}

            static inline bool
            compare_nodes_z (ORROctree::Node* node1, ORROctree::Node* node2)
            {
              return static_cast<bool> (node1->getData ()->get3dIdZ () < node2->getData ()->get3dIdZ ());
            }

            inline void
            insert (ORROctree::Node* leaf) { nodes_.insert(leaf);}

            inline std::set<ORROctree::Node*, bool(*)(ORROctree::Node*,ORROctree::Node*)>&
            get_nodes (){ return nodes_;}

            inline int
            get_x () const { return x_;}

            inline int
            get_y () const { return y_;}

          protected:
            std::set<ORROctree::Node*, bool(*)(ORROctree::Node*,ORROctree::Node*)> nodes_;
            int x_, y_;
        };

      public:
        ORROctreeZProjection ()
        : pixels_(NULL),
          sets_(NULL)
        {}
        virtual ~ORROctreeZProjection (){}

        void
        build (const ORROctree& input, float eps_front, float eps_back);

        void
        clear ();

        inline void
        getPixelCoordinates (const float* p, int& x, int& y)
        {
        	x = static_cast<int> ((p[0] - bounds_[0])*inv_pixel_size_);
        	y = static_cast<int> ((p[1] - bounds_[2])*inv_pixel_size_);
        }

        inline Pixel*
        getPixel (const float* p)
        {
          int x, y; this->getPixelCoordinates (p, x, y);

          if ( 0 < x || x >= num_pixels_x_ ) return (NULL);
          if ( 0 < y || y >= num_pixels_y_ ) return (NULL);

          return (pixels_[x][y]);
        }

      protected:
        float pixel_size_, inv_pixel_size_, bounds_[4], extent_x_, extent_y_;
        int num_pixels_x_, num_pixels_y_, num_pixels_;
        Pixel ***pixels_;
        Set ***sets_;
        std::list<Set*> full_sets_;
    };
  } // namespace recognition
} // namespace pcl


#endif /* ORR_OCTREE_ZPROJECTION_H_ */