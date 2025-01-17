	/*===========================================================================*\
 | 
 |  FILE:	UVWFrame.h
 |			Helper object that derives Mapping coordinates from scene objects
 |			3D Studio MAX R3.0
 | 
 |  AUTH:   Diego Castaño
 |			Copyright(c) Mankua 2000
 |
 |  HIST:	Started 1-1-00
 | 
\*===========================================================================*/

#ifndef __HELPSKEL__H
#define __HELPSKEL__H

#include "max.h"

#if MAX_VERSION_MAJOR < 9
#include "max_mem.h"
#endif

#include "iparamm2.h"
#include "resource.h"


// IMPORTANT:
// JW: removed direct project dependency
//#include "..\..\texlay\code\texlay.h"

// this classID is shared with TexLayers code
#define	UVWFRAME_CLASSID		Class_ID(0x388676bb, 0x7752389b)

TCHAR *GetString(int id);
extern ClassDesc* GetUVWFrameDesc();

#define PBLOCK_REF		0
#define UVW_MESH_REF	1

// Paramblock2 name
enum { shelp_params, }; 
// Paramblock2 parameter list
enum { pb_length, pb_width, pb_height, pb_uvwmesh, };

class PickNodeMode;

class UVWFace {
public:
	int deg;
	int mat_id;
	int * vtx; 
	};

class UVWFrameObject : public HelperObject
{
public:

	int creating;

	int selectedInMankua;

	IParamBlock2 *pblock2;
	static IObjParam *ip;

	INode *uvwMesh;

	// Snap suspension flag (TRUE during creation only)
	BOOL suspendSnap;

	float simple;
	int extDispFlags;

	UVWFrameObject();
	~UVWFrameObject();

	// From BaseObject
	int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt);
	void Snap(TimeValue t, INode* inode, SnapInfo *snap, IPoint2 *p, ViewExp *vpt);
	void SetExtendedDisplay(int flags);
	int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags);
	CreateMouseCallBack* GetCreateMouseCallBack();
	void DrawBox(TimeValue t, INode *inode, ViewExp *vpt, BOOL hit_testing);

	void BeginEditParams(IObjParam *ip, ULONG flags, Animatable *prev);
	void EndEditParams(IObjParam *ip, ULONG flags, Animatable *next);
	void InvalidateUI();

	// From Object
	ObjectState Eval(TimeValue time);

	void InitNodeName(TSTR& s) { s = GetString(IDS_UVWFRAME_CLASSNAME); }
	ObjectHandle ApplyTransform(Matrix3& matrix) { return this; }
	Interval ObjectValidity(TimeValue t);

	// We don't convert to anything
	int CanConvertToType(Class_ID obtype) { return FALSE; }
	Object* ConvertToType(TimeValue t, Class_ID obtype) { assert(0); return NULL; }

	void GetWorldBoundBox(TimeValue t, INode *mat, ViewExp *vpt, Box3& box);
	void GetLocalBoundBox(TimeValue t, INode *mat, ViewExp *vpt, Box3& box);
	int DoOwnSelectHilite() { return 1; }

	// Animatable methods
	void DeleteThis() { delete this; }
	Class_ID ClassID() { return UVWFRAME_CLASSID; }

#if MAX_VERSION_MAJOR < 24
	void GetClassName(TSTR& s) { s = GetString(IDS_UVWFRAME_CLASSNAME); }
#else
	void GetClassName(TSTR& s, bool localized = false) const
	{
		UNUSED_PARAM(localized);
		s = GetString(IDS_UVWFRAME_CLASSNAME);
	}

#endif

	int IsKeyable() { return 0; }

	// Direct paramblock access
	int	NumParamBlocks() { return 1; }
	IParamBlock2* GetParamBlock(int i) { return pblock2; }
	IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock2->ID() == id) ? pblock2 : NULL; }

	int NumSubs() { return 1; }
	Animatable* SubAnim(int i);

#if MAX_VERSION_MAJOR < 24
	TSTR SubAnimName(int i);
#else
	TSTR SubAnimName(int i, bool localized = false);
#endif

	// From ref
	int NumRefs() { return 2; }
	RefTargetHandle GetReference(int i);
#if MAX_VERSION_MAJOR < 14
	void SetReference(int i, RefTargetHandle rtarg);
#else
private:
	virtual void SetReference(int i, RefTargetHandle rtarg);
public:
#endif

#if MAX_VERSION_MAJOR < 15 //Max 2013
	TCHAR *GetObjectName() { return GetString(IDS_UVWFRAME_CLASSNAME); }
#else
	const TCHAR *GetObjectName(bool localized = false) const
	{
		UNUSED_PARAM(localized);
		return GetString(IDS_UVWFRAME_CLASSNAME);
	}

#endif

#if MAX_VERSION_MAJOR < 9 
		RefTargetHandle Clone(RemapDir &remap = NoRemap());
#else
		RefTargetHandle Clone(RemapDir &remap);
#endif

#if MAX_VERSION_MAJOR < 17 //Max 2015
		RefResult NotifyRefChanged( Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message );
#else
		RefResult NotifyRefChanged( const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate );
#endif		
		BOOL AddUVWMesh(INode *node);
		void SetNodeName();
		
		void SaveUVW(HWND hWnd);
		void LoadUVW(HWND hWnd);
	
		virtual float GetLength(TimeValue t);
		virtual float GetWidth(TimeValue t);
		virtual float GetHeight(TimeValue t);

		virtual void SetLength(TimeValue t,float val);
		virtual void SetWidth(TimeValue t,float val);
		virtual void SetHeight(TimeValue t,float val);
		virtual void SetNode(TimeValue t,INode * node);

		// Entregarle el UVW Data a el TexLay
		virtual void GetMatID(Tab <int> &matids, TimeValue t);
		virtual void GetUVW( BOOL &uvwData, Tab <Point3> &uvw_verts, Tab <UVWFace> &uvw_faces, Matrix3 ftm, TimeValue t );
		virtual void SelectInMankua( int sel );
};				

class UVWFrameCreateCallBack : public CreateMouseCallBack
{
	UVWFrameObject *sho;
	IPoint2 sp0,sp1;
	Point3 p0;

	public:
	
	int proc( ViewExp *vpt, int msg, int point, int flags, IPoint2 m, Matrix3& mat);
	void SetObj(UVWFrameObject *obj) {sho = obj;}
};

/*===========================================================================*\
 |	Dialog Processor
\*===========================================================================*/

class UVWFrameDlgProc : public ParamMap2UserDlgProc 
{
	public:
		UVWFrameObject *sho;

		UVWFrameDlgProc() {}
		UVWFrameDlgProc(UVWFrameObject *sho_in) { sho = sho_in; }

		//WIN32 : BOOL DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void DeleteThis() { }

		void SetThing(ReferenceTarget *m) {
			sho = (UVWFrameObject*)m;
			}

};

#endif
