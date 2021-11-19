#pragma once
#include <string>
#include <vector>
#include "Transform.h"
#include "Fbx.h"
#include "Direct3D.h"

namespace Model
{
	struct ModelData
	{
		Fbx* pFbx;
		Transform transform;
		std::string fileName;
	};

	



	//���f�������[�h
	//�����FfileName�@�t�@�C����
	//�ߒl�F���̃��f���f�[�^�Ɋ��蓖�Ă�ꂽ�ԍ�
	int Load(std::string fileName, SHADER_TYPE shaderType);

	//���[���h�s���ݒ�
	//�����Fhandle		�ݒ肵�������f���̔ԍ�
	//�����Ftransform	�ړ��E��]�E�g����
	//�ߒl�F�Ȃ�
	void SetTransform(int handle, Transform transform);


	void Draw(int handle);


	void AllRelease();

	void RayCast(int handle, RayCastData* rayData);
};

