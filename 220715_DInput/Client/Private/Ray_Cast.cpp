#include "stdafx.h"
#include "Ray_Cast.h"

//IMPLEMENT_SINGLETON(CRay_Cast)

//CRay_Cast::CRay_Cast()
//{
//}
//
//_bool CRay_Cast::RayCasting(CTransform * pTransform, _float3 * pOut)
//{
//	_float4x4 WorldMatrixInv = pTransform->Get_WorldMatrixInverse();
//
//	_float3 vRayPos = pTransform->Get_State(CTransform::STATE_POSITION);
//
//	_float3 vRayDir;
//
//	pPicking->Compute_LocalRayInfo(&vRayDir, &vRayPos, pTransform);
//
//	D3DXVec3Normalize(&vRayDir, &vRayDir);
//
//	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
//	{
//		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
//		{
//			_uint		iIndex = i * m_iNumVerticesX + j;
//
//			_uint		iIndices[] = {
//				iIndex + m_iNumVerticesX,
//				iIndex + m_iNumVerticesX + 1,
//				iIndex + 1,
//				iIndex
//			};
//
//			_float		fU, fV, fDist;
//			_float4x4	WorldMatrix = pTransform->Get_WorldMatrix();
//
//			/* 오른쪽 상단. */
//			if (true == D3DXIntersectTri(&m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]], &vRayPos, &vRayDir, &fU, &fV, &fDist))
//			{
//				_float3	vPickPos = vRayPos + vRayDir * fDist;
//
//				D3DXVec3TransformCoord(pOut, &vPickPos, &WorldMatrix);
//
//				return true;
//			}
//
//			/* 왼쪽 하단. */
//			if (true == D3DXIntersectTri(&m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[2]], &m_pVerticesPos[iIndices[3]], &vRayPos, &vRayDir, &fU, &fV, &fDist))
//			{
//				_float3	vPickPos = vRayPos + vRayDir * fDist;
//
//				D3DXVec3TransformCoord(pOut, &vPickPos, &WorldMatrix);
//
//				return true;
//			}
//		}
//	}
//
//	return false;
//}
//
//void CRay_Cast::Free()
//{
//}
