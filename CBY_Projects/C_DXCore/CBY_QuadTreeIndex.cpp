#include "CBY_QuadTreeIndex.h"

void CBY_QuadTreeIndex::CreateLODIndexData(int maxcells)
{
	CBY_StaticData* pStaticData = new CBY_StaticData;
	pStaticData->dwLevel = 0;
	pStaticData->m_LODIndexList.resize(1);

	//Level 0 생성
	for (DWORD dwCol = 0; dwCol < maxcells; dwCol++)
	{
		for (DWORD dwRow = 0; dwRow < maxcells; dwRow++)
		{
			DWORD dwRowNext = dwRow + 1;
			DWORD dwColNext = dwCol + 1;

			pStaticData->m_LODIndexList[0].push_back(dwCol * (maxcells+1) + dwRow);
			pStaticData->m_LODIndexList[0].push_back(dwCol * (maxcells + 1) + dwRowNext);
			pStaticData->m_LODIndexList[0].push_back(dwColNext * (maxcells + 1) + dwRow);
											   
			pStaticData->m_LODIndexList[0].push_back(dwColNext * (maxcells + 1) + dwRow);
			pStaticData->m_LODIndexList[0].push_back(dwCol * (maxcells + 1) + dwRowNext);
			pStaticData->m_LODIndexList[0].push_back(dwColNext * (maxcells + 1) + dwRowNext);
		}
	}

	m_StaticDataList.push_back(pStaticData);
	CreateIndexBuffer(m_StaticDataList[0]);

	//Level1 ~ 최대 Level까지
	for (DWORD dwLevel = 0; dwLevel < m_dwLodCount; dwLevel++)
	{
		DWORD dwOffset = (DWORD)(pow(2.0f, float(dwLevel) + 1));		//레벨에 따라 오프셋이 2^n다. 다만 0레벨은 위에서 만들었으므로 2^n+1로 계산중
		DWORD dwNumCell = (maxcells / dwOffset);						//셀의 갯수는 최대 셀에서 오프셋 만큼 사각형의 거리가 늘어나고 셀이 줄었으니 전체셀/오프셋이다.

		CBY_StaticData* pStaticData = new CBY_StaticData;
		pStaticData->dwLevel = dwLevel+1;
		pStaticData->m_LODIndexList.resize(16);							//레벨 1부터는 총 16가지의 경우의 수를 따져서 미리 그에따른 인덱스 버퍼를 생성한다.

		for (DWORD dwData = 0; dwData < 16; dwData++)
		{
			//DWORD dwIndexCounter = 
		}
	}
}

HRESULT CBY_QuadTreeIndex::CreateIndexBuffer(CBY_StaticData* pdata)
{
	HRESULT hr = S_OK;
	pdata->IndexBufferList.resize(pdata->m_LODIndexList.size());
	for (int iData = 0; iData < pdata->m_LODIndexList.size(); iData++)
	{
		if (pdata->m_LODIndexList[iData].size() <= 0) break;
		pdata->IndexBufferList[iData].Attach(CDXH::CreateIndexBuffer(m_pMap->m_obj.m_pd3dDevice, &pdata->m_LODIndexList[iData].at(0),
			(UINT)pdata->m_LODIndexList[iData].size(),
			sizeof(DWORD)));
	}
	return hr;
}

bool CBY_QuadTreeIndex::Release()
{
	CBY_QuadTree::Release();
	
	for (int i = 0; i < m_StaticDataList.size(); i++)
	{
		delete m_StaticDataList[i];
	}
	m_StaticDataList.clear();
	return true;
}

CBY_QuadTreeIndex::CBY_QuadTreeIndex()
{
}


CBY_QuadTreeIndex::~CBY_QuadTreeIndex()
{
}

CBY_StaticData::CBY_StaticData()
{

}
CBY_StaticData::~CBY_StaticData()
{

}