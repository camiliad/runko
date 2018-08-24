#include <iostream>

#include "tile.h"



/* 
 * 1D version:
 *	ey(i,j,k)=ey(i,j,k) + c *(bz(im1,j,k)-bz(i,j,k))  
 *	ez(i,j,k)=ez(i,j,k) + c *(by(i,j,k)-by(im1,j,k)) 
 *
 * 2D version
 * ex(i,j,k)=ex(i,j,k)+const*(-bz(i,jm1,k)+bz(i,j,k))
 * ey(i,j,k)=ey(i,j,k)+const*(bz(im1,j,k)-bz(i,j,k))
 * ez(i,j,k)=ez(i,j,k)+const*(bx(i,jm1,k)-bx(i,j,k)-  by(im1,j,k)+by(i,j,k))
*/

/*! \brief Update E field with full step
 *
 * Contains a dimension switch for solvers depending on internal mesh dimensions
 */


/// 1D E pusher
template<>
void fields::Tile<1>::pushE() {

  fields::YeeLattice& mesh = getYee();
  Realf C = 1.0 * cfl;

  int k = 0;
  int j = 0;
  for(int i=0; i<static_cast<int>(mesh_lengths[0]); i++) {

    // Ex
    // NONE

    // Ey
    mesh.ey(i,j,k) += 
      + C*( mesh.bz(i-1,j, k  ) - mesh.bz(i,j,k));

    // Ez
    mesh.ez(i,j,k) += 
      + C*(-mesh.by(i-1,j,   k) + mesh.by(i,j,k));

  }

}

/// 2D E pusher
template<>
void fields::Tile<2>::pushE() {

  fields::YeeLattice& mesh = getYee();

  Realf C = 1.0 * cfl;

  int k = 0;
  for(int j=0; j<static_cast<int>(mesh_lengths[1]); j++) 
  for(int i=0; i<static_cast<int>(mesh_lengths[0]); i++) {

    // Ex
    mesh.ex(i,j,k) += 
      + C*(-mesh.bz(i,j-1,k  ) + mesh.bz(i,j,k));

    // Ey
    mesh.ey(i,j,k) += 
      + C*( mesh.bz(i-1,j, k  ) - mesh.bz(i,j,k));

    // Ez
    mesh.ez(i,j,k) += 
      + C*( mesh.bx(i,  j-1, k) - mesh.bx(i,j,k) 
          -mesh.by(i-1,j,   k) + mesh.by(i,j,k));

  }
}


/// 3D E pusher
template<>
void fields::Tile<3>::pushE() {

  fields::YeeLattice& mesh = getYee();
  Realf C = 1.0 * cfl;

  for(int k=0; k<static_cast<int>(mesh_lengths[2]); k++)
  for(int j=0; j<static_cast<int>(mesh_lengths[1]); j++)
  for(int i=0; i<static_cast<int>(mesh_lengths[0]); i++) {

    // Ex
    mesh.ex(i,j,k) += 
      + C*( mesh.by(i,j,  k-1) - mesh.by(i,j,k))
      + C*(-mesh.bz(i,j-1,k  ) + mesh.bz(i,j,k));

    // Ey
    mesh.ey(i,j,k) += 
      + C*( mesh.bz(i-1,j, k  ) - mesh.bz(i,j,k))
      + C*(-mesh.bx(i,  j, k-1) + mesh.bx(i,j,k));

    // Ez
    mesh.ez(i,j,k) += 
      + C*( mesh.bx(i,  j-1, k) - mesh.bx(i,j,k))
      + C*(-mesh.by(i-1,j,   k) + mesh.by(i,j,k));

  }

}


//--------------------------------------------------


/// Deposit current into electric field
template<std::size_t D>
void fields::Tile<D>::depositCurrent() {
  fields::YeeLattice& mesh = getYee();

  mesh.ex -= mesh.jx;
  mesh.ey -= mesh.jy;
  mesh.ez -= mesh.jz;

}

//--------------------------------------------------

/*
 * 1D version:
  by(i,j,k)=by(i,j,k)+const*(ez(ip1,j,k)-ez(i,j,k)) !-0*ex(i,j,k+1)+0*ex(i,j,k))
	bz(i,j,k)=bz(i,j,k)+const*(ey(i,j,k)-ey(ip1,j,k)) !+0*ex(i,j+1,k)-0*ex(i,j,k))
  
 * 2D version:
 * bx(i,j,k)=bx(i,j,k)+const*(-ez(i,jp1,k)+ez(i,j,k))
 * by(i,j,k)=by(i,j,k)+const*(ez(ip1,j,k)-ez(i,j,k))
 * bz(i,j,k)=bz(i,j,k)+const*(ex(i,jp1,k)-ex(i,j,k) -ey(ip1,j,k)+ey(i,j,k))
*/

/// Update B field with a half step


/// 1D B pusher
template<>
void fields::Tile<1>::pushHalfB() {
  fields::YeeLattice& mesh = getYee();
  Realf C = 0.5 * cfl;

  int k = 0;
  int j = 0;
  for(int i=0; i<static_cast<int>(mesh_lengths[0]); i++) {

    // Bx
    // NONE

    // By
    mesh.by(i,j,k) += 
      + C*( mesh.ez(i+1,j, k  ) - mesh.ez(i,j,k));

    // Bz
    mesh.bz(i,j,k) += 
      + C*(-mesh.ey(i+1,j,   k) + mesh.ey(i,j,k));
  }

}

/// 2D B pusher
template<>
void fields::Tile<2>::pushHalfB() {
  fields::YeeLattice& mesh = getYee();

  Realf C = 0.5 * cfl;

  int k = 0;
  for(int j=0; j<static_cast<int>(mesh_lengths[1]); j++)
  for(int i=0; i<static_cast<int>(mesh_lengths[0]); i++) {

    // Bx
    mesh.bx(i,j,k) += 
      + C*(-mesh.ez(i,  j+1,k  ) + mesh.ez(i,j,k));

    // By
    mesh.by(i,j,k) += 
      + C*( mesh.ez(i+1,j, k  ) - mesh.ez(i,j,k));

    // Bz
    mesh.bz(i,j,k) += 
      + C*( mesh.ex(i,  j+1, k) - mesh.ex(i,j,k)
          -mesh.ey(i+1,j,   k) + mesh.ey(i,j,k));

  }

}


/// 3D B pusher
template<>
void fields::Tile<3>::pushHalfB() {
  fields::YeeLattice& mesh = getYee();
  Realf C = 0.5 * cfl;

  for(int k=0; k<static_cast<int>(mesh_lengths[2]); k++) 
  for(int j=0; j<static_cast<int>(mesh_lengths[1]); j++) 
  for(int i=0; i<static_cast<int>(mesh_lengths[0]); i++) {

    // Bx
    mesh.bx(i,j,k) += 
      + C*( mesh.ey(i,  j,  k+1) - mesh.ey(i,j,k))
      + C*(-mesh.ez(i,  j+1,k  ) + mesh.ez(i,j,k));

    // By
    mesh.by(i,j,k) += 
      + C*( mesh.ez(i+1,j, k  ) - mesh.ez(i,j,k))
      + C*(-mesh.ex(i,  j, k+1) + mesh.ex(i,j,k));

    // Bz
    mesh.bz(i,j,k) += 
      + C*( mesh.ex(i,  j+1, k) - mesh.ex(i,j,k))
      + C*(-mesh.ey(i+1,j,   k) + mesh.ey(i,j,k));

  }
}




/// Get current time snapshot of Yee lattice
template<std::size_t D>
fields::YeeLattice& fields::Tile<D>::getYee(size_t i) {
  return this->yee.get(i);
}


/// Get analysis lattice of i:th species
template<std::size_t D>
fields::PlasmaMomentLattice& fields::Tile<D>::getAnalysis(size_t i) {
  return this->analysis.at(i);
}

//--------------------------------------------------
// Specialize analysis species grid extension
template<>
void fields::Tile<1>::addAnalysisSpecies() {
  analysis.emplace_back(mesh_lengths[0], 1, 1);
}

template<>
void fields::Tile<2>::addAnalysisSpecies() {
  analysis.emplace_back(mesh_lengths[0], mesh_lengths[1], 1);
}

template<>
void fields::Tile<3>::addAnalysisSpecies() {
  analysis.emplace_back(mesh_lengths[0], mesh_lengths[1], mesh_lengths[2]);
}


//--------------------------------------------------
// Specialize Yee Lattice insertion
template<>
void fields::Tile<1>::addYeeLattice() {
  yee.push_back( fields::YeeLattice( mesh_lengths[0], 1, 1) );
}

template<>
void fields::Tile<2>::addYeeLattice() {
  yee.push_back( fields::YeeLattice( mesh_lengths[0], mesh_lengths[1], 1) );
}

template<>
void fields::Tile<3>::addYeeLattice() {
  yee.push_back( fields::YeeLattice( mesh_lengths[0], mesh_lengths[1], mesh_lengths[2]) );
}

//--------------------------------------------------

/// Quick helper function to copy everything inside Yee lattice 
void copyVertYee(
    fields::YeeLattice& lhs, 
    fields::YeeLattice& rhs, 
    int lhsI, int rhsI) {

  lhs.ex.copyVert(rhs.ex, lhsI, rhsI); 
  lhs.ey.copyVert(rhs.ey, lhsI, rhsI); 
  lhs.ez.copyVert(rhs.ez, lhsI, rhsI); 

  lhs.bx.copyVert(rhs.bx, lhsI, rhsI); 
  lhs.by.copyVert(rhs.by, lhsI, rhsI); 
  lhs.bz.copyVert(rhs.bz, lhsI, rhsI); 

}


/// Quick helper function to add everything inside Yee lattice 
void addVertYee(
    fields::YeeLattice& lhs, 
    fields::YeeLattice& rhs, 
    int lhsI, int rhsI) {

  lhs.jx.addVert(rhs.jx, lhsI, rhsI); 
  lhs.jy.addVert(rhs.jy, lhsI, rhsI); 
  lhs.jz.addVert(rhs.jz, lhsI, rhsI); 

}



/// Quick helper function to copy everything inside Yee lattice 
void copyHorzYee(
    fields::YeeLattice& lhs, 
    fields::YeeLattice& rhs, 
    int lhsJ, int rhsJ) {

  lhs.ex.copyHorz(rhs.ex, lhsJ, rhsJ); 
  lhs.ey.copyHorz(rhs.ey, lhsJ, rhsJ); 
  lhs.ez.copyHorz(rhs.ez, lhsJ, rhsJ); 
                                    
  lhs.bx.copyHorz(rhs.bx, lhsJ, rhsJ); 
  lhs.by.copyHorz(rhs.by, lhsJ, rhsJ); 
  lhs.bz.copyHorz(rhs.bz, lhsJ, rhsJ); 

}


/// Quick helper function to add everything inside Yee lattice 
void addHorzYee(
    fields::YeeLattice& lhs, 
    fields::YeeLattice& rhs, 
    int lhsJ, int rhsJ) {
                                    
  lhs.jx.addHorz(rhs.jx, lhsJ, rhsJ); 
  lhs.jy.addHorz(rhs.jy, lhsJ, rhsJ); 
  lhs.jz.addHorz(rhs.jz, lhsJ, rhsJ); 
}

/// Quick helper function to copy everything inside Yee lattice 
void copyFaceYee(
    fields::YeeLattice& lhs, 
    fields::YeeLattice& rhs, 
    int lhsK, int rhsK) {

  lhs.ex.copyFace(rhs.ex, lhsK, rhsK); 
  lhs.ey.copyFace(rhs.ey, lhsK, rhsK); 
  lhs.ez.copyFace(rhs.ez, lhsK, rhsK); 
                                    
  lhs.bx.copyFace(rhs.bx, lhsK, rhsK); 
  lhs.by.copyFace(rhs.by, lhsK, rhsK); 
  lhs.bz.copyFace(rhs.bz, lhsK, rhsK); 

}


/// Quick helper function to add everything inside Yee lattice 
void addFaceYee(
    fields::YeeLattice& lhs, 
    fields::YeeLattice& rhs, 
    int lhsK, int rhsK) {
                                    
  lhs.jx.addFace(rhs.jx, lhsK, rhsK); 
  lhs.jy.addFace(rhs.jy, lhsK, rhsK); 
  lhs.jz.addFace(rhs.jz, lhsK, rhsK); 
}



void copyZdirPencilYee(
    fields::YeeLattice& lhs, 
    fields::YeeLattice& rhs, 
    int lhsI, int lhsJ,
    int rhsI, int rhsJ) {

  lhs.ex.copyZdirPencil(rhs.ex, lhsI, lhsJ, rhsI, rhsJ); 
  lhs.ey.copyZdirPencil(rhs.ey, lhsI, lhsJ, rhsI, rhsJ); 
  lhs.ez.copyZdirPencil(rhs.ez, lhsI, lhsJ, rhsI, rhsJ); 

  lhs.bx.copyZdirPencil(rhs.bx, lhsI, lhsJ, rhsI, rhsJ); 
  lhs.by.copyZdirPencil(rhs.by, lhsI, lhsJ, rhsI, rhsJ); 
  lhs.bz.copyZdirPencil(rhs.bz, lhsI, lhsJ, rhsI, rhsJ); 

}

void addZdirPencilYee(
    fields::YeeLattice& lhs, 
    fields::YeeLattice& rhs, 
    int lhsI, int lhsJ,
    int rhsI, int rhsJ) {

  lhs.jx.addZdirPencil(rhs.jx, lhsI, lhsJ, rhsI, rhsJ); 
  lhs.jy.addZdirPencil(rhs.jy, lhsI, lhsJ, rhsI, rhsJ); 
  lhs.jz.addZdirPencil(rhs.jz, lhsI, lhsJ, rhsI, rhsJ); 

}




/// Update Yee grid boundaries
template<>
void fields::Tile<1>::updateBoundaries(corgi::Node<1>& node) {

  // target
  fields::YeeLattice& mesh = getYee();

  // left 
  auto cleft = 
    std::dynamic_pointer_cast<fields::Tile<1> >(
        node.getTilePtr( neighs(-1) ));
  fields::YeeLattice& mleft = cleft->getYee();

  // copy from right side to left
  copyVertYee(mesh, mleft, -1, mleft.Nx-1); 

  // right
  auto cright = 
    std::dynamic_pointer_cast<fields::Tile<1> >(
        node.getTilePtr( neighs(+1) ));
  fields::YeeLattice& mright = cright->getYee();
    
  // copy from left side to right
  copyVertYee(mesh, mright, mesh.Nx, 0); 

}


/// Update Yee grid boundaries
template<>
void fields::Tile<2>::updateBoundaries(corgi::Node<2>& node) {

  // target
  fields::YeeLattice& mesh = getYee();
  int halo = 1; // halo region size for current

  // left 
  auto cleft = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(-1, 0) ));
  fields::YeeLattice& mleft = cleft->getYee();

  // copy from right side to left
  for(int h=1; h<= halo; h++)
  copyVertYee(mesh, mleft, -h, mleft.Nx-h); 


  // right
  auto cright = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(+1, 0) ));
  fields::YeeLattice& mright = cright->getYee();
    
  // copy from left side to right
  for(int h=1; h<= halo; h++)
  copyVertYee(mesh, mright, mesh.Nx+h-1, h-1); 


  // TODO: fix these: they produce saw-like oscillations
  // top 
  auto ctop = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(0, +1) ));
  fields::YeeLattice& mtop = ctop->getYee();

  //copy from bottom side to top
  for(int h=1; h<= halo; h++)
  copyHorzYee(mesh, mtop, mesh.Ny+h-1, h-1); 


  // bottom
  auto cbot = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(0, -1) ));
  fields::YeeLattice& mbot = cbot->getYee();
    
  // copy from top side to bottom
  for(int h=1; h<= halo; h++)
  copyHorzYee(mesh, mbot, -h, mbot.Ny-h); 


  // --------------------------------------------------  
  // diagonals
  // TODO: loop over H

  auto ctopleft = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(-1, +1) ));
  fields::YeeLattice& mtopleft = ctopleft->getYee();

  for(int h=1; h<= halo; h++)
  for(int g=1; g<= halo; g++)
  copyZdirPencilYee(mesh, mtopleft, -h,           mesh.Ny +g-1,
                                   mtopleft.Nx-h, +g-1);

  auto ctopright = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(+1, +1) ));
  fields::YeeLattice& mtopright = ctopright->getYee();

  for(int h=1; h<= halo; h++)
  for(int g=1; g<= halo; g++)
  copyZdirPencilYee(mesh, mtopright, mesh.Nx +h-1, mesh.Ny +g-1,
                                     +h-1,         +g-1);

  auto cbotleft = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(-1, -1) ));
  fields::YeeLattice& mbotleft = cbotleft->getYee();

  for(int h=1; h<= halo; h++)
  for(int g=1; g<= halo; g++)
  copyZdirPencilYee(mesh, mbotleft, -h, -g,
                          mbotleft.Nx-h, mbotleft.Ny-g);

  auto cbotright = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(+1, -1) ));
  fields::YeeLattice& mbotright = cbotright->getYee();

  for(int h=1; h<= halo; h++)
  for(int g=1; g<= halo; g++)
  copyZdirPencilYee(mesh, mbotright, mesh.Nx +h-1, -g,
                                     +h-1,         mbotright.Ny-g);


  // --------------------------------------------------  
  // front
  // TODO: hack to deal with 2D corgi tiles
  //copyFaceYee(mesh, mesh, -1, mesh.Nz-1);

  // back
  //copyFaceYee(mesh, mesh, mesh.Nz, 0);
  // --------------------------------------------------  
  // TODO: x-pencils
  // TODO: y-pencils
  // TODO: corners


}

template<>
void fields::Tile<1>::exchangeCurrents(corgi::Node<1>& node) {

  // target
  fields::YeeLattice& mesh = getYee();

  int halo = 1; // halo region size for currents


  // left 
  auto cleft = 
    std::dynamic_pointer_cast<fields::Tile<1> >(
        node.getTilePtr( neighs(-1) ));
  fields::YeeLattice& mleft = cleft->getYee();

  // add from right side to left
  for(int h=1; h<= halo; h++)
  addVertYee(mesh, mleft, -h, mleft.Nx-h); 


  // right
  auto cright = 
    std::dynamic_pointer_cast<fields::Tile<1> >(
        node.getTilePtr( neighs(+1) ));
  fields::YeeLattice& mright = cright->getYee();
    
  // add from left side to right
  for(int h=1; h<= halo; h++)
  addVertYee(mesh, mright, mesh.Nx+h-1, h-1); 

}


/// Update currents on Yee grid boundaries
// TODO: assumes implicitly 2D (x-y) arrays only by setting k=0 and then ignoring it
// TODO: write unit test for this
template<>
void fields::Tile<2>::exchangeCurrents(corgi::Node<2>& node) {

  // target
  fields::YeeLattice& mesh = getYee();

  int halo = 3;

  // left 
  auto cleft = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(-1, 0) ));
  fields::YeeLattice& mleft = cleft->getYee();

  // add from left to right
  for(int h=0; h< halo; h++) addVertYee(mesh, mleft, h, mleft.Nx+h); 

  // right
  auto cright = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(+1, 0) ));
  fields::YeeLattice& mright = cright->getYee();
    
  // add from right to left
  for(int h=1; h<= halo; h++) addVertYee(mesh, mright, mesh.Nx-h, -h); 


  // top 
  auto ctop = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(0, +1) ));
  fields::YeeLattice& mtop = ctop->getYee();

  //add from top to bottom
  for(int h=0; h< halo; h++) addHorzYee(mesh, mtop, h, mtop.Ny+h); 


  // bottom
  auto cbot = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(0, -1) ));
  fields::YeeLattice& mbot = cbot->getYee();
    
  // add from bottom to top
  for(int h=1; h<=halo; h++) addHorzYee(mesh, mbot, mesh.Ny-h, -h); 



  // --------------------------------------------------  
  // diagonals
  auto ctopleft = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(-1, +1) ));
  fields::YeeLattice& mtopleft = ctopleft->getYee();

  for(int h=0; h<  halo; h++)
  for(int g=1; g<= halo; g++)
  addZdirPencilYee(mesh, mtopleft, h, mesh.Ny-g,
                                   mtopleft.Nx+h, -g);


  auto ctopright = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(+1, +1) ));
  fields::YeeLattice& mtopright = ctopright->getYee();

  for(int h=1; h<= halo; h++)
  for(int g=1; g<= halo; g++)
  addZdirPencilYee(mesh, mtopright, mesh.Nx-h, mesh.Ny-g,
                                    -h,         -g);

  auto cbotleft = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(-1, -1) ));
  fields::YeeLattice& mbotleft = cbotleft->getYee();
  
  for(int h=0; h<  halo; h++)
  for(int g=0; g<  halo; g++)
  addZdirPencilYee(mesh, mbotleft, h,            g,
                                   mbotleft.Nx+h, mbotleft.Ny+g);

  auto cbotright = 
    std::dynamic_pointer_cast<fields::Tile<2> >(
        node.getTilePtr( neighs(+1, -1) ));
  fields::YeeLattice& mbotright = cbotright->getYee();

  for(int h=1; h<= halo; h++)
  for(int g=0; g<  halo; g++)
  addZdirPencilYee(mesh, mbotright, mesh.Nx-h, g,
                                    -h,      mbotright.Ny+g);


  // front
  // TODO: hack to deal with 2D corgi tiles
  //addFaceYee(mesh, mesh, -1, mesh.Nz-1);

  // back
  //addFaceYee(mesh, mesh, mesh.Nz, 0);

}

template<std::size_t D>
void fields::Tile<D>::cycleYee() {
  yee.cycle();
}

/// cycle temporary and true current arrays
template<std::size_t D>
void fields::Tile<D>::cycleCurrent() 
{
  auto& yee = this->getYee();

  std::swap( yee.jx.mat, yee.jx1.mat );
  std::swap( yee.jy.mat, yee.jy1.mat );
  std::swap( yee.jz.mat, yee.jz1.mat );

}


//--------------------------------------------------
// explicit template instantiation

template class fields::Tile<1>;
template class fields::Tile<2>;
//template class fields::Tile<3>;
