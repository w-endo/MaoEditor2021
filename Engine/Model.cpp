#include "Model.h"

namespace Model
{
    std::vector<ModelData*> datas;
}

int Model::Load(std::string fileName)
{
    ModelData* pModelData = new ModelData;
    pModelData->fileName = fileName;

    bool isExist = false;
    for (int i = 0; i < datas.size(); i++)
    {
        if (datas[i]->fileName == fileName)
        {
            pModelData->pFbx = datas[i]->pFbx;
            isExist = true;
            break;
        }
    }

    if (!isExist)
    {
        pModelData->pFbx = new Fbx;
        pModelData->pFbx->Load(fileName);
    }

    datas.push_back(pModelData);

    return datas.size()-1;
}

void Model::SetTransform(int handle, Transform transform)
{
    datas[handle]->transform = transform;
}

void Model::Draw(int handle)
{
    datas[handle]->pFbx->Draw(datas[handle]->transform);
}

void Model::AllRelease()
{
    for (int i = 0; i < datas.size(); i++)
    {
        datas[i]->pFbx->Release();
        if (datas[i]->pFbx) {
            delete(datas[i]->pFbx);
        }

        for (int j = i + 1; j < datas.size(); j++)
        {
            if (datas[i]->fileName == datas[j]->fileName)
            {
                datas[j]->pFbx = nullptr;
            }
        }

        delete(datas[i]);
    }
    datas.clear();
}

void Model::RayCast(int handle, RayCastData* rayData)
{
    //‚Æ‚è‚ ‚¦‚¸ƒxƒNƒgƒ‹‚É‚µ‚Ä‚¨‚­
    XMVECTOR start = XMLoadFloat3(&rayData->start); //”­ŽËˆÊ’u
    XMVECTOR dir = XMLoadFloat3(&rayData->dir);     //”­ŽË•ûŒü

    //‡@
    XMMATRIX matInv = XMMatrixInverse(nullptr, datas[handle]->transform.GetWorldMatrix());

    //‡A
    XMVECTOR target = start + dir;

    //‡B
    start = XMVector3TransformCoord(start, matInv);

    //‡C
    target = XMVector3TransformCoord(target, matInv);

    //‡D
    XMStoreFloat3(&rayData->start, start);

    //‡E
    XMStoreFloat3(&rayData->dir, target - start);

    datas[handle]->pFbx->RayCast(rayData);
}