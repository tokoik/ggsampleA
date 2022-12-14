#version 410 core

//
// simple.vert
//
//   単純な陰影付けを行ってオブジェクトを描画するシェーダ
//

// 光源
layout (std140) uniform Light
{
  vec4 lamb;                        // 環境光成分
  vec4 ldiff;                       // 拡散反射光成分
  vec4 lspec;                       // 鏡面反射光成分
  vec4 lpos;                        // 位置
};

// 材質
layout (std140) uniform Material
{
  vec4 kamb;                        // 環境光の反射係数
  vec4 kdiff;                       // 拡散反射係数
  vec4 kspec;                       // 鏡面反射係数
  float kshi;                       // 輝き係数
};

// 変換行列
uniform mat4 mp;                    // 投影変換行列
uniform mat4 mv;                    // モデルビュー変換行列
uniform mat4 mn;                    // 法線変換行列

// オブジェクトの回転
uniform mat4 rotate;

// オブジェクトの並進
uniform mat4 translate;

// 頂点属性
layout (location = 0) in vec4 pv;   // ローカル座標系における頂点の位置
layout (location = 1) in vec4 nv;   // ローカル座標系における頂点の法線ベクトル

// ラスタライザに送る頂点属性
out vec4 idiff;                     // 拡散反射光強度
out vec4 ispec;                     // 鏡面反射光強度

void main(void)
{
  // 視点座標系における頂点の位置
  vec4 p = mv * pv;

  // 視点座標系における視線ベクトル
  vec3 v = -normalize(vec3(p));

  // 視点座標系における光源の位置
  vec4 q = mv * lpos;

  // 視点座標系における光線ベクトル
  vec3 l = normalize(vec3(q * p.w - q.w * p));

  // 視点座標系における中間ベクトル
  vec3 h = normalize(l + v);

  // 視点座標系における法線ベクトル
  vec3 n = normalize(vec3(mn * nv));

  // 陰影計算
  idiff = max(dot(n, l), 0.0) * kdiff * ldiff + kamb * lamb;
  ispec = pow(max(dot(n, h), 0.0), kshi) * kspec * lspec;

  // クリッピング座標系における頂点の位置
  gl_Position = mp * p;
}
