#include "EngineFMesh.h"
#include "AllStruct.h"

EngineFMesh::EngineFMesh()
{
}

EngineFMesh::~EngineFMesh()
{
}

int EngineFMesh::GetSize()
{
	return sizeof(vertices[0]) + sizeof(indices[0]);
}



void EngineFMesh::init(FbxNode* pNode)
{
	tempmatrix = pNode->EvaluateGlobalTransform();
	FbxMesh* pMesh = pNode->GetMesh();
	if (pMesh == nullptr) {
		assert(false);
	}

	FBuffer buffer;
	int PolygonCount = pMesh->GetPolygonCount();

	FbxLayerElementUV* uvs = pMesh->GetLayer(0)->GetUVs();

	int totalVertexCount = 0;

	if (uvs->GetMappingMode() == FbxLayerElement::eByPolygonVertex) {

		for (int i = 0; i < PolygonCount; ++i) {
			int polygonSize = pMesh->GetPolygonSize(i); // 이 폴리곤이 몇 개의 정점을 가지는지

			for (int j = 0; j < polygonSize - 2; ++j) {		//index 추출
				indices.push_back(totalVertexCount + 0);
				indices.push_back(totalVertexCount + j + 1);
				indices.push_back(totalVertexCount + j + 2);
			}

			for (int j = 0; j < polygonSize; ++j) {
				int index = pMesh->GetPolygonVertex(i, j);
				FbxVector4 pos = pMesh->GetControlPointAt(index);
				pos = tempmatrix.MultT(pos);			//여러개의 메쉬가 있을 때 위치 이동
				buffer.position = DirectX::XMFLOAT3(static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]));
				buffer.controlpointindex = index;		//버텍스가 여러개지만, 인덱스상 같은 위치에 있을 수 있음 컨트롤 포인트 상 위치
															// 컨트롤 포인트는 버텍스 버퍼가 아니라 실제 겹치는 점을 제외한 모든 점의 개수
				//uvIndex는 PolygonVertex 기준으로 계산
				int polygonVertexIndex = totalVertexCount + j;

				int uvIndex = uvs->GetIndexArray().GetAt(polygonVertexIndex);

				if (uvs->GetReferenceMode() == FbxLayerElement::eDirect) {
					uvIndex = polygonVertexIndex;
				}
				else if (uvs->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
					uvIndex = uvs->GetIndexArray().GetAt(polygonVertexIndex);
				}
				else {
					assert(false && "Unsupported ReferenceMode for UVs");
				}

				FbxVector2 uv = uvs->GetDirectArray().GetAt(uvIndex);
				buffer.uv = DirectX::XMFLOAT2(static_cast<float>(uv[0]), static_cast<float>(uv[1]));
				buffer.uv.y = 1.0f - buffer.uv.y;
				vertices.push_back(buffer);
			}
			totalVertexCount += polygonSize;
		}
	}
	else {
		assert(false);
	}

}