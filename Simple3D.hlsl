//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D		g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[
Texture2D		g_textureShadow : register(t1);	//�e�N�X�`���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
	float4x4	matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	matNormal;		//�@����ό`������s��
	float4x4	matWorld;
	float4x4	g_mWLP;   //���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����̍��� 
	float4x4	g_mWLPT;   //���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����EUV �s��̍���
	float4	 diffuseColor;
	float4	 speculer;
	float4	 camPos;
	float4	 lightPos;
	float	 shininess;
	bool		isTexture;
	float	scroll;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos  : SV_POSITION;	//�ʒu
	float2 uv	: TEXCOORD;		//UV���W
	float4 color	: COLOR;	//�F�i���邳�j
	float4 LightTexCoord :  TEXCOORD5;
	float4 LighViewPos :  TEXCOORD6;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;

	//�@������]
	normal = mul(normal, matNormal);
	normal = normalize(normal);

	float4 light = normalize(lightPos);// float4(-1, 1, -1, 0);
	light = normalize(light);
	outData.color = clamp(dot(normal, light), 0, 1);

	//���C�g�r���[���Q�Ƃ���Ƃ��A�肪����ƂȂ�e�N�X�`���[���W 
	outData.LightTexCoord = mul(pos, g_mWLPT);    //���̓_���A���C�g�r���[�ł������Ƃ��̈ʒu���킩�� 

	//���C�g�r���[�ɂ�����ʒu(�ϊ���) 
	outData.LighViewPos = mul(pos, matWorld);

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	float4 diffuse;
	float4 ambient;
	//if (isTexture)
	//{
	//	diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.color;
	//	ambient = g_texture.Sample(g_sampler, inData.uv) * float4(0.2, 0.2, 0.2, 1);
	//	diffuse.a = g_texture.Sample(g_sampler, inData.uv).a;
	//}
	//else
	{
		diffuse = diffuseColor * inData.color;
		ambient = diffuseColor * float4(0.2, 0.2, 0.2, 1);
		diffuse.a = 1;
	}

	//�e�̏��� 
	inData.LightTexCoord /= inData.LightTexCoord.w;
	float TexValue = g_textureShadow.Sample(g_sampler, inData.LightTexCoord).r;
	float LightLength = length(inData.LighViewPos - lightPos)/30.0;
	if (TexValue+0.005 < LightLength)  //���C�g�r���[�ł̒������Z���i���C�g�r���[�ł͎Օ���������j 
	{
		diffuse *=0.0;  //�e�i���邳�� 1/3�j 
	}
	//diffuse = LightLength;
	diffuse.a = 1;
	//return diffuse;



	//diffuse = g_textureShadow.Sample(g_sampler, inData.LightTexCoord);
	//diffuse.a = 1;

	return diffuse + ambient;
	
}
