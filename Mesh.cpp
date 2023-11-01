#include "Mesh.h"

void Mesh::Initilize(DirectXCommon* directX)
{
	directX = directX_;
	CreateMesh();

}



void Mesh::CreateMesh()
{
	const uint32_t kSubdivision = 16;
	uint32_t latIndex = 0;
	uint32_t lonIndex = 0;
	float MPI = 3.14f;
	const float kLonEvrey = 2 * float(MPI) / float(kSubdivision);//φd//横の分割数
	const float kLatEvrey = float(MPI) / float(kSubdivision);//θd//縦の分割数
	float Vertexsphere = kSubdivision * kSubdivision * 6;
	VertexData* vertexData = nullptr;


	//球体の描画
	for (int latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		//θ
		float lat = -float(MPI) / 2.0f + kLatEvrey * latIndex;
		//経度の方向に分割しながら線を描く
		for (int lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;

			//texcoord専用のxy座標
			float u = float(lonIndex) / float(kSubdivision);
			//下から0上が1になっていたので「1.0f-～」にして逆にする
			float v = 1.0f - float(latIndex) / float(kSubdivision);

			float lon = lonIndex * kLonEvrey;
			//頂点にデータを入力する。基準点a
			//点間の距離
			float length = 1.0f / kSubdivision;

#pragma region 三角形１枚目 
			//左上(点B)が原点
			//abc
			//資料通りだとここは点a(左下)
			vertexData[start].position.x = (cos(lat) * cos(lon));
			vertexData[start].position.y = (sin(lat));
			vertexData[start].position.z = (cos(lat) * sin(lon));
			vertexData[start].position.w = 1.0f;
			//分割分移動
			vertexData[start].texcoord.x = u;
			vertexData[start].texcoord.y = v + length;

			//点b(左上)
			vertexData[start + 1].position.x = (cos(lat + kLatEvrey)) * cos(lon);
			vertexData[start + 1].position.y = (sin(lat + kLatEvrey));
			vertexData[start + 1].position.z = (cos(lat + kLatEvrey)) * sin(lon);
			vertexData[start + 1].position.w = 1.0f;
			vertexData[start + 1].texcoord.x = u;
			vertexData[start + 1].texcoord.y = v;


			//点c(右下)
			vertexData[start + 2].position.x = (cos(lat) * cos(lon + kLonEvrey));
			vertexData[start + 2].position.y = (sin(lat));
			vertexData[start + 2].position.z = (cos(lat) * sin(lon + kLonEvrey));
			vertexData[start + 2].position.w = 1.0f;
			vertexData[start + 2].texcoord.x = u + length;
			vertexData[start + 2].texcoord.y = v + length;

#pragma endregion

#pragma region 三角形２枚目
			//bcd
			//点d(右上)
			vertexData[start + 3].position.x = (cos(lat + kLatEvrey) * cos(lon + kLonEvrey));
			vertexData[start + 3].position.y = (sin(lat + kLatEvrey));
			vertexData[start + 3].position.z = (cos(lat + kLatEvrey) * sin(lon + kLonEvrey));
			vertexData[start + 3].position.w = 1.0f;
			vertexData[start + 3].texcoord.x = u + length;
			vertexData[start + 3].texcoord.y = v;

			//点c(右下)
			vertexData[start + 4].position.x = (cos(lat) * cos(lon + kLonEvrey));
			vertexData[start + 4].position.y = (sin(lat));
			vertexData[start + 4].position.z = (cos(lat) * sin(lon + kLonEvrey));
			vertexData[start + 4].position.w = 1.0f;
			vertexData[start + 4].texcoord.x = u + length;
			vertexData[start + 4].texcoord.y = v + length;



			//点b(左上)
			vertexData[start + 5].position.x = (cos(lat + kLatEvrey) * cos(lon));
			vertexData[start + 5].position.y = (sin(lat + kLatEvrey));
			vertexData[start + 5].position.z = (cos(lat + kLatEvrey) * sin(lon));
			vertexData[start + 5].position.w = 1.0f;
			vertexData[start + 5].texcoord.x = u;
			vertexData[start + 5].texcoord.y = v;
#pragma endregion
		}
	}


}