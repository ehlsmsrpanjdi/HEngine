#include "EngineFMesh.h"
#include "AllStruct.h"
#include "EngineAnimation.h"

EngineFMesh::EngineFMesh()
{
}

EngineFMesh::~EngineFMesh()
{
}

UINT EngineFMesh::GetSize()
{
	return sizeof(FBuffer);
}

UINT EngineFMesh::GetArraySize()
{
	return static_cast<UINT>(vertices.size());
}

const std::vector<FBuffer>& EngineFMesh::GetVertex()
{
	return vertices;
}

UINT EngineFMesh::GetIndexSize() {
	return sizeof(UINT);
}
UINT EngineFMesh::GetIndexArraySize() {
	return static_cast<UINT>(indices.size());
}

const std::vector<UINT>& EngineFMesh::GetIndices(){
	return indices;
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
			int polygonSize = pMesh->GetPolygonSize(i); // �� �������� �� ���� ������ ��������

			for (int j = 0; j < polygonSize - 2; ++j) {		//index ����
				indices.push_back(totalVertexCount + 0);
				indices.push_back(totalVertexCount + j + 1);
				indices.push_back(totalVertexCount + j + 2);
			}

			for (int j = 0; j < polygonSize; ++j) {
				int index = pMesh->GetPolygonVertex(i, j); //�̰� 
				/*
				* n��° �������� m��° ������ ����Ű�� ��Ʈ�� ����Ʈ �ε����� ��ȯ�ϴµ�
				* control points�� ���� ��� ������ ��ġ ��ǥ�� (�ߺ�����)
				*  polygon vertex � ��Ʈ�� ����Ʈ�� ����ؼ� �������� �������� ���������� ���� �Ѵٰ��ϴµ�
				*  getpolygonvertex(n,m)�� �� �������� m���� �������� control point�� �� ���� ����ϴ°�? �� ���� ����
				*/


				FbxVector4 pos = pMesh->GetControlPointAt(index);
				//pos = tempmatrix.MultT(pos);			//�������� �޽��� ���� �� ��ġ �̵�
				buffer.position = DirectX::XMFLOAT3(static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]));
				buffer.controlpointindex = index;		//���ؽ��� ����������, �ε����� ���� ��ġ�� ���� �� ���� ��Ʈ�� ����Ʈ �� ��ġ
															// ��Ʈ�� ����Ʈ�� ���ؽ� ���۰� �ƴ϶� ���� ��ġ�� ���� ������ ��� ���� ����
															// �̰� index�� �����ϴ� ������ �ش� ���۰� �� ��°�� ���� ����Ǿ��ִ��� �˱� ���ؼ�
															// 
				//uvIndex�� PolygonVertex �������� ���
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