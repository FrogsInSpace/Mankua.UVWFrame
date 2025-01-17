/*===========================================================================*\
 | 
 |  FILE:	frame_mod.h
 |			A simple modifier that gets mapping coordinates from
 |			the UVW Frame helper object
 |			3D Studio MAX R3.0
 | 
 |  AUTH:   Diego A. Castaño
 |			Mankua
 |			Copyright(c) Mankua 2001
 |
 |  HIST:	Started 6-6-2001
 | 
\*===========================================================================*/

#ifndef __FRAME_MOD__H
#define __FRAME_MOD__H

#include "max.h"
#include "iparamm2.h"
#include "istdplug.h"
#include "meshadj.h"
#include "modstack.h"
#include "macrorec.h"
#include "resource.h"

// IMPORTANT:
// The ClassID must be changed whenever a new project
// is created using this skeleton
#define	PUREM_CLASSID		Class_ID(0x3e446f14, 0x63605fb4)

TCHAR *GetString(int id);
extern ClassDesc* GetUVWFrameModDesc();

// Paramblock2 name
enum { frame_mod_params, }; 
// Paramblock2 parameter list
enum { uvw_type, uvw_channel, frame_node };

/*===========================================================================*\
 |	UVWFrameModifier class defn
\*===========================================================================*/

class UVWFrameModifier : public Modifier{
	public:

		// Access to the interface
		static IObjParam *ip;

		// Global parameter block
		IParamBlock2	*pblock;

		//Constructor/Destructor
		UVWFrameModifier();
		~UVWFrameModifier() {}
		void DeleteThis() { delete this; }

		// Plugin identification
#if MAX_VERSION_MAJOR < 24		
		void GetClassName(TSTR& s) { s= TSTR(GetString(IDS_FRAMEMOD_CLASSNAME)); }  
#else				
		void GetClassName(TSTR& s, bool localized=false ) const { s = TSTR(GetString(IDS_FRAMEMOD_CLASSNAME)); }
#endif		
		
		
		virtual Class_ID ClassID() { return PUREM_CLASSID;}		
#if MAX_VERSION_MAJOR < 15 //Max 2013
		TCHAR *GetObjectName() { return GetString(IDS_FRAMEMOD_CLASSNAME); }
#elif MAX_VERSION_MAJOR < 24
		const TCHAR *GetObjectName() { return GetString(IDS_FRAMEMOD_CLASSNAME); }
#else
		const TCHAR *GetObjectName( bool localized = false ) const { return GetString(IDS_FRAMEMOD_CLASSNAME); }
#endif
		// Defines the behavior for this modifier
		// This is currently setup to be basic geometry 
		// modification of deformable objects
		ChannelMask ChannelsUsed()  { return PART_GEOM|PART_TOPO|PART_TEXMAP|PART_VERTCOLOR; }
		ChannelMask ChannelsChanged() { return PART_GEOM|PART_TOPO|PART_TEXMAP|PART_VERTCOLOR; }
		Class_ID InputType() { return defObjectClassID; }
		BOOL ChangeTopology() {return FALSE;}

		// Calculate the local validity from the parameters
		Interval LocalValidity(TimeValue t);
		Interval GetValidity(TimeValue t);

		// Object modification and notification of change
		void ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node);
		void NotifyInputChanged(Interval changeInt, PartID partID, RefMessage message, ModContext *mc);

		// Reference support
		int NumRefs() { return 1; }
		RefTargetHandle GetReference(int i);
#if MAX_VERSION_MAJOR < 14
		void SetReference(int i, RefTargetHandle rtarg);
#else
private:
		virtual void SetReference(int i, RefTargetHandle rtarg);
public:
#endif

#if MAX_VERSION_MAJOR < 9
		RefTargetHandle Clone(RemapDir& remap = NoRemap());
#else
		RefTargetHandle Clone(RemapDir& remap);
#endif

#if MAX_VERSION_MAJOR < 17 //Max 2015
		RefResult NotifyRefChanged( Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message );
#else
		RefResult NotifyRefChanged( const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate );
#endif		
		
		// SubAnim support
		int NumSubs() { return 1; }
		Animatable* SubAnim(int i);
		
#if MAX_VERSION_MAJOR < 24		
		TSTR SubAnimName(int i);
#else
		TSTR SubAnimName(int i, bool localized = false);
#endif


		// Direct paramblock access
		int	NumParamBlocks() { return 1; }	
		IParamBlock2* GetParamBlock(int i) { return pblock; }
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; }
		int GetParamBlockIndex(int id) {return id;}

		// Does not use createmouse callbacks
		CreateMouseCallBack* GetCreateMouseCallBack() {return NULL;}

		// Load and unload our UI
		void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
		void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);
		void InvalidateUI();
};


/*===========================================================================*\
 |	Dialog Processor
\*===========================================================================*/

class SkeletonPureModDlgProc : public ParamMap2UserDlgProc 
{
	public:
		UVWFrameModifier *mod;

		SkeletonPureModDlgProc() {}
		SkeletonPureModDlgProc(UVWFrameModifier *mod_in) { mod = mod_in; }

		//Win32 : BOOL DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void DeleteThis() { }

		void SetThing(ReferenceTarget *m) {
			mod = (UVWFrameModifier*)m;
			}

};

#endif
