#define AROS_ALMOST_COMPATIBLE 1

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/intuition.h>
#include <proto/boopsi.h>
#include <proto/graphics.h>
#include <proto/cybergraphics.h>
#include <exec/memory.h>
#include <intuition/screens.h>
#include <intuition/icclass.h>
#include <intuition/cghooks.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <graphics/gfx.h>
#include <cybergraphx/cybergraphics.h>

#include <string.h>

#include "asl_intern.h"
#include "layout.h"
#include "coolimages.h"

#define SDEBUG 0
#define DEBUG 0

#include <aros/debug.h>

#define G(x) ((struct Gadget *)(x))
#define EG(x) ((struct ExtGadget *)(x))

#define CLASS_ASLBASE ((struct AslBase_intern *)cl->cl_UserData)
#define HOOK_ASLBASE  ((struct AslBase_intern *)hook->h_Data)

#define AslBase CLASS_ASLBASE

/********************** ASL PROP CLASS **************************************************/

struct AslPropData
{
    Object *frame;
};

/*********************
**  AslProp::New()  **
*********************/
static IPTR aslprop_new(Class * cl, Object * o, struct opSet * msg)
{
    struct AslPropData *data;
    struct TagItem fitags[] =
    {
	{IA_FrameType, FRAME_BUTTON},
	{TAG_DONE, 0UL}
    };
    
    o = (Object *)DoSuperMethodA(cl, o, (Msg)msg);
    if (o)
    {
    	data = INST_DATA(cl, o);

	EG(o)->BoundsLeftEdge = G(o)->LeftEdge;
	EG(o)->BoundsTopEdge  = G(o)->TopEdge;
	EG(o)->BoundsWidth    = G(o)->Width;
	EG(o)->BoundsHeight   = G(o)->Height;
	EG(o)->MoreFlags |= GMORE_BOUNDS;

	G(o)->LeftEdge += BORDERPROPSPACINGX;
	G(o)->TopEdge  += BORDERPROPSPACINGY;
	G(o)->Width    -= BORDERPROPSPACINGX * 2;
	G(o)->Height   -= BORDERPROPSPACINGY * 2;
		
	data->frame = NewObjectA(NULL, FRAMEICLASS, fitags);
	if (!data->frame)
	{
	    CoerceMethod(cl, o, OM_DISPOSE);
	    o = NULL;
	}
    }

    return (IPTR)o;
}

/*************************
**  AslProp::Dispose()  **
*************************/
static IPTR aslprop_dispose(Class * cl, Object * o, Msg msg)
{
    struct AslPropData *data;
    IPTR retval;
    
    data = INST_DATA(cl, o);
    if (data->frame) DisposeObject(data->frame);
    
    retval = DoSuperMethodA(cl, o, msg);
    
    return retval;
}

/*********************
**  AslProp::Set()  **
*********************/
static IPTR aslprop_set(Class * cl, Object * o, struct opSet * msg)
{
    struct AslPropData *data;
    struct TagItem *tstate = msg->ops_AttrList, *ti;
    IPTR retval;
    BOOL rerender = FALSE;
    
    data = INST_DATA(cl, o);
    
    while((ti = NextTagItem(&tstate)))
    {
        switch(ti->ti_Tag)
	{
	    case ASLSC_Inc:
	    case ASLSC_Dec:
	        if (((LONG)ti->ti_Data > 0) &&
		    (((struct opUpdate *)msg)->opu_Flags & OPUF_INTERIM))
		{
		     IPTR top, total, visible, newtop;
		     
		     GetAttr(PGA_Top,     o, &top);
		     GetAttr(PGA_Total,   o, &total);
		     GetAttr(PGA_Visible, o, &visible);
		     
		     newtop = top;
		     if (ti->ti_Data == ID_ARROWDEC)
		     {
		         if (top > 0) newtop--;
		     } else {
		         if (top < total - visible) newtop++;
		     }
		     		     
		     if (newtop != top)
		     {
		         struct TagItem set_tags [] =
			 {
			     {PGA_Top, newtop 	},
			     {TAG_DONE		}
			 };
			 struct opSet ops;
			 
			 ops.MethodID  	  = OM_SET;
			 ops.ops_AttrList = set_tags;
			 ops.ops_GInfo 	  = msg->ops_GInfo;
			 
			 DoMethodA(o, (Msg)&ops);
			 
			 /* rerender = TRUE; */
		     }
		     
		} /* if ((ti->ti_Data > 0) && (((struct opUpdate *)msg)->opu_Flags & OPUF_INTERIM)) */
				
	} /* switch(ti->ti_Tag) */

    } /* while((ti = NextTagItem(&tstate))) */
    
    retval = DoSuperMethodA(cl, o, (Msg) msg);
 
    if (rerender)
    {
        struct RastPort *rp;
	
	rp = ObtainGIRPort(msg->ops_GInfo);
    	if (NULL != rp)
	{
	    DoMethod(o, GM_RENDER, msg->ops_GInfo, rp, GREDRAW_UPDATE);
	    ReleaseGIRPort(rp);
	}
    }
      
    return retval;
}

/************************
**  AslProp::Render()  **
************************/
static IPTR aslprop_render(Class *cl, Object *o, struct gpRender *msg)
{
    struct AslPropData *data;
    IPTR retval;
    
    data = INST_DATA(cl, o);
    
    if (msg->gpr_Redraw == GREDRAW_REDRAW)
    {
        struct TagItem im_tags[] =
	{
	    {IA_Width	, 0	},
	    {IA_Height	, 0	},
	    {TAG_DONE		}
	};	
	WORD x, y, w, h;
	
	getgadgetcoords(G(o), msg->gpr_GInfo, &x, &y, &w, &h);
	
	x -= BORDERPROPSPACINGX;
	y -= BORDERPROPSPACINGY;
	w += BORDERPROPSPACINGX * 2;
	h += BORDERPROPSPACINGY * 2;
	
	im_tags[0].ti_Data = w;
	im_tags[1].ti_Data = h;
	
	SetAttrsA(data->frame, im_tags);
	
	DrawImageState(msg->gpr_RPort,
		       (struct Image *)data->frame,
		       x,
		       y,
		       IDS_NORMAL,
		       msg->gpr_GInfo->gi_DrInfo);

    } /* if (msg->gpr_Redraw == GREDRAW_REDRAW) */
    
    retval = DoSuperMethodA(cl, o, (Msg)msg);
    
    return retval;
}

AROS_UFH3S(IPTR, dispatch_aslpropclass,
	  AROS_UFHA(Class *, cl, A0),
	  AROS_UFHA(Object *, obj, A2),
	  AROS_UFHA(Msg, msg, A1)
)
{
    IPTR retval = 0UL;

    switch (msg->MethodID)
    {
        case OM_NEW:
	    retval = aslprop_new(cl, obj, (struct opSet *)msg);
	    break;
	
	case OM_DISPOSE:
	    retval = aslprop_dispose(cl, obj, msg);
	    break;
	
	case OM_SET:
	case OM_UPDATE:
	    retval = aslprop_set(cl, obj, (struct opSet *)msg);
	    break;
	
	case GM_RENDER:
	    retval = aslprop_render(cl, obj, (struct gpRender *)msg);
	    break;
	    
	default:
	    retval = DoSuperMethodA(cl, obj, msg);
	    break;

    } /* switch (msg->MethodID) */
    
    return retval;
}

/********************** ASL ARROW CLASS **************************************************/

struct AslArrowData
{
    WORD scrollticker;
};

/*************************
**  AslArrow::Notify()  **
*************************/
static IPTR aslarrow_notify(Class * cl, Object * o, struct opUpdate *msg)
{
    struct AslArrowData *data;
    IPTR retval = 0;
    
    data = INST_DATA(cl, o);

    if (!data->scrollticker || (data->scrollticker == SCROLLTICKER))
    {
        retval = DoSuperMethodA(cl, o, (Msg)msg);
    }
    
    if (data->scrollticker) data->scrollticker--;

    return retval;
}

/***************************
**  AslArrow::Goactive()  **
***************************/
static IPTR aslarrow_goactive(Class * cl, Object * o, struct gpInput *msg)
{
    struct AslArrowData *data;
    IPTR retval;
    
    data = INST_DATA(cl, o);
    data->scrollticker = SCROLLTICKER;
    
    retval = DoSuperMethodA(cl, o, (Msg)msg);
    
    return retval;
}

AROS_UFH3S(IPTR, dispatch_aslarrowclass,
	  AROS_UFHA(Class *, cl, A0),
	  AROS_UFHA(Object *, obj, A2),
	  AROS_UFHA(Msg, msg, A1)
)
{
    IPTR retval = 0UL;

    switch (msg->MethodID)
    {
	case OM_NOTIFY:
	    retval = aslarrow_notify(cl, obj, (struct opUpdate *)msg);
	    break;
	    
	case GM_GOACTIVE:
	    retval = aslarrow_goactive(cl, obj, (struct gpInput *)msg);
	    break;
	        
	default:
	    retval = DoSuperMethodA(cl, obj, msg);
	    break;

    } /* switch (msg->MethodID) */
    
    return retval;
}

/***********************************************************************************/

struct AslListViewData
{
    Object		*frame;
    struct LayoutData 	*ld;
    struct List		*labels;
    struct Node		**nodetable;
    struct List		emptylist;
    struct Hook		default_renderhook;
    struct Hook		*renderhook;
    ULONG  		clicksec;
    ULONG		clickmicro;
    WORD		minx;
    WORD   		miny;
    WORD		maxx;
    WORD		maxy;
    WORD		width;
    WORD		height;
    WORD		itemheight;
    WORD		spacing;
    WORD		lineheight;
    WORD		visible;
    WORD		top;
    WORD		total;
    WORD		active;
    WORD		rendersingleitem;
    BYTE		layouted;
    BYTE		doubleclicked;
    BYTE		domultiselect;
    BYTE		multiselecting;
};

#undef AslBase
#define AslBase HOOK_ASLBASE

/************************
**  ASLLVRenderHook()  **
************************/
AROS_UFH3(IPTR, ASLLVRenderHook,
    AROS_UFHA(struct Hook *,            hook,     	A0),
    AROS_UFHA(struct Node *,    	node,           A2),
    AROS_UFHA(struct ASLLVDrawMsg *,	msg,	        A1)
)
{
    IPTR retval;

    if (msg->lvdm_MethodID == LV_DRAW)
    {
    	struct DrawInfo *dri = msg->lvdm_DrawInfo;
    	struct RastPort *rp  = msg->lvdm_RastPort;
    	
    	WORD min_x = msg->lvdm_Bounds.MinX;
    	WORD min_y = msg->lvdm_Bounds.MinY;
    	WORD max_x = msg->lvdm_Bounds.MaxX;
    	WORD max_y = msg->lvdm_Bounds.MaxY;

        UWORD erasepen = BACKGROUNDPEN;
	UWORD textpen = TEXTPEN;

     	SetDrMd(rp, JAM1);
     	    
    	
     	switch (msg->lvdm_State)
     	{
     	    case ASLLVR_SELECTED:
		erasepen = FILLPEN;
		textpen = FILLTEXTPEN;
		
		/* Fall through */
		
     	    case ASLLVR_NORMAL:
	    {
    	    	WORD numfit;
    	    	struct TextExtent te;
    	    
		SetAPen(rp, dri->dri_Pens[erasepen]);
     	    	RectFill(rp, min_x, min_y, max_x, max_y);
     	    	
		if (node) if (node->ln_Name)
		{
    	    	    numfit = TextFit(rp,
				     node->ln_Name,
				     strlen(node->ln_Name),
    	    			     &te,
				     NULL,
				     1,
				     max_x - min_x + 1 - BORDERLVITEMSPACINGX * 2, 
				     max_y - min_y + 1);

	    	    SetAPen(rp, dri->dri_Pens[textpen]);

    	    	    /* Render text */
    	    	    Move(rp, min_x + BORDERLVITEMSPACINGX,
			     min_y + BORDERLVITEMSPACINGY + rp->Font->tf_Baseline);
    	    	    Text(rp, node->ln_Name, numfit);
	    	}
     	    	
     	    } break;
       	}
     	
     	retval = ASLLVCB_OK;
     }
     else
     {
     	retval = ASLLVCB_UNKNOWN;
     }
     	
     return retval;
}

#undef AslBase
#define AslBase CLASS_ASLBASE

static struct Node *findnode(Class *cl, Object *o, WORD which)
{
    struct AslListViewData *data;
    struct Node *node = NULL;

    data = INST_DATA(cl, o);
    
    if (data->nodetable)
    {
        if (which < data->total) node = data->nodetable[which];
    } else {
        node = FindListNode(data->labels, which);
    }
    
    return node;
}

static void makenodetable(Class *cl, Object *o)
{
    struct AslListViewData *data;
    
    data = INST_DATA(cl, o);
    
    if (data->nodetable)
    {
        FreeVec(data->nodetable);
        data->nodetable = NULL;
    }
    
    /* data->total must be correct here */
    
    if (data->total > 0)
    {
        if ((data->nodetable = AllocVec(sizeof(struct Node *) * data->total, MEMF_PUBLIC)))
	{
	    struct Node *node, **nodeptr = data->nodetable;
	    ForeachNode(data->labels, node)
	    {
	        *nodeptr++ = node;
	    }
	}
    }
}

static void renderitem(Class *cl, Object *o, struct Node *node, WORD liney, struct RastPort *rp)
{
    struct AslListViewData *data;
    struct ASLLVDrawMsg msg;
    
    data = INST_DATA(cl, o);
    
    msg.lvdm_MethodID = ASLLV_DRAW;
    msg.lvdm_RastPort = rp;
    msg.lvdm_DrawInfo = data->ld->ld_Dri;
    msg.lvdm_Bounds.MinX  = data->minx + BORDERLVSPACINGX;
    msg.lvdm_Bounds.MaxX  = data->maxx - BORDERLVSPACINGX;
    msg.lvdm_Bounds.MinY  = data->miny + BORDERLVSPACINGY + liney * data->lineheight;
    msg.lvdm_Bounds.MaxY  = msg.lvdm_Bounds.MinY + data->lineheight - 1;    
    msg.lvdm_State = node ? (IS_SELECTED(node) ? ASLLVR_SELECTED : ASLLVR_NORMAL) : ASLLVR_NORMAL;
    
    CallHookPkt(data->renderhook, node, &msg);
}

static void renderallitems(Class *cl, Object *o, struct RastPort *rp)
{
    struct AslListViewData *data;
    struct Node *node;
    WORD i;
    
    data = INST_DATA(cl, o);
    
    node = findnode(cl, o, data->top);
    
    for(i = 0; i < data->visible;i ++)
    {
        if (node) if (!node->ln_Succ) node = NULL;

        renderitem(cl, o, node , i, rp);

	if (node) node = node->ln_Succ;
    }   
}

static void rendersingleitem(Class *cl, Object *o, struct GadgetInfo *gi, WORD which)
{
    struct AslListViewData *data;

    data = INST_DATA(cl, o);

    if ((which >= data->top) &&
        (which < data->top + data->visible) &&
	(which < data->total))
    {
	struct RastPort *rp;
    	struct Node *node;

	node = findnode(cl, o, which);

	if ((rp = ObtainGIRPort(gi)))
	{		    
	    struct gpRender gpr;

	    data->rendersingleitem = which;

	    gpr.MethodID   = GM_RENDER;
	    gpr.gpr_GInfo  = gi;
	    gpr.gpr_RPort  = rp;
	    gpr.gpr_Redraw = GREDRAW_UPDATE;

	    DoMethodA(o, (Msg)&gpr);

	    data->rendersingleitem = -1;

	    ReleaseGIRPort(rp);

	} /* if ((rp = ObtainGIRPort(msg->gpi_GInfo))) */

    } /* if ((which >= data->top) && ... */

}

static WORD mouseitem(Class *cl, Object *o, WORD mousex, WORD mousey)
{
    struct AslListViewData *data;
    WORD result = -5;
    
    data = INST_DATA(cl, o);

    if (mousey < BORDERLVSPACINGY)
    {
        result = -1;
    } else if (mousey > data->maxy - data->miny - BORDERLVSPACINGY)
    {
        result = -2;
    } else if (mousex < BORDERLVSPACINGX)
    {
        result = -3;
    } else if (mousex > data->maxx - data->minx - BORDERLVSPACINGX)
    {
        result = -4;
    } else {
        WORD i = (mousey - BORDERLVSPACINGY) / data->lineheight;

	if (i < data->visible)
	{
            struct Node *node;

	    i += data->top;
	 
	    if ((node = findnode(cl, o, i)))
	    {
	        result = i;
	    }
	}
    }
    
    return result;
}

static void notifyall(Class *cl, Object *o, struct GadgetInfo *gi, STACKULONG flags)
{
    struct AslListViewData * data = INST_DATA(cl, o);
    struct TagItem tags[] =
    {
        {ASLLV_Top	, data->top 	},
	{ASLLV_Total	, data->total	},
	{ASLLV_Visible	, data->visible },
	{TAG_DONE			}
    };
    struct opUpdate opu;
    
    opu.MethodID = OM_NOTIFY;
    opu.opu_AttrList = tags;
    opu.opu_GInfo = gi;
    opu.opu_Flags = flags;
    
    D(bug("asl listview notify: top = %d  total = %d  visible  = %d\n",
    	 data->top,
	 data->total,
	 data->visible));
	 
    DoSuperMethodA(cl, o, (Msg)&opu);
    
};

/*************************
**  AslListView::Set()  **
*************************/
static IPTR asllistview_set(Class * cl, Object * o, struct opSet * msg)
{
    struct AslListViewData *data;
    struct TagItem *tag, *tstate = msg->ops_AttrList;
    IPTR retval;
    BOOL redraw = FALSE;
    WORD newtop;
    
    data = INST_DATA(cl, o);

    retval = DoSuperMethodA(cl, o, (Msg) msg);

    while((tag = NextTagItem(&tstate)))
    {
        switch(tag->ti_Tag)
	{
            case ASLLV_Top:
		newtop = tag->ti_Data;
		if (newtop + data->visible > data->total)
		{
	            newtop = data->total - data->visible;
		}
		if (newtop < 0) newtop = 0;

		if (newtop != data->top)
		{
	            data->top = newtop;
	            redraw = TRUE;
		}
		break;
	
	    case ASLLV_Labels:
	    	data->labels = tag->ti_Data ? (struct List *)tag->ti_Data : &data->emptylist;
		data->total = CountNodes(data->labels, 0);
		data->active = -1;
		
		if (!data->layouted) data->visible = data->total;
		
		if (data->top + data->visible > data->total)
		{
		    data->top = data->total - data->visible;
		}
		if (data->top < 0) data->top = 0;
		if (!data->layouted) data->visible = data->total;
		
		makenodetable(cl, o);
		
		notifyall(cl, o, msg->ops_GInfo, 0);
		
		redraw = TRUE;
		break;

	} /* switch(tag->ti_Tag) */
	 
    } /* while((tag = NextTagItem(&tsate))) */

    
    if (redraw)
    {
        struct RastPort *rp;
	struct gpRender gpr;
	
	if ((rp = ObtainGIRPort(msg->ops_GInfo)))
	{
	    gpr.MethodID   = GM_RENDER;
	    gpr.gpr_GInfo  = msg->ops_GInfo;
	    gpr.gpr_RPort  = rp;
	    gpr.gpr_Redraw = GREDRAW_UPDATE;
	    
	    DoMethodA(o, (Msg)&gpr);
	    
	    ReleaseGIRPort(rp);
	}
    }
    
    return retval;
}

/*************************
**  AslListView::New()  **
*************************/
static IPTR asllistview_new(Class * cl, Object * o, struct opSet * msg)
{
    struct AslListViewData *data;
    struct TagItem fitags[] =
    {
	{IA_FrameType, FRAME_BUTTON},
	{IA_EdgesOnly, TRUE	   },
	{TAG_DONE, 0UL}
    };
    
    o = (Object *)DoSuperMethodA(cl, o, (Msg)msg);
    if (o)
    {
    	data = INST_DATA(cl, o);

	/* We want to get a GM_LAYOUT message, no matter if gadget is GFLG_RELRIGHT/RELBOTTOM/
	   RELWIDTH/RELHEIGHT or not */	   
	G(o)->Flags |= GFLG_RELSPECIAL;
	
	data->frame = NewObjectA(NULL, FRAMEICLASS, fitags);
	data->ld = (struct LayoutData *)GetTagData(GA_UserData, 0, msg->ops_AttrList);
	
	if (!data->ld || !data->frame)
	{
	    CoerceMethod(cl, o, OM_DISPOSE);
	    o = NULL;
	} else {
	   data->itemheight = GetTagData(ASLLV_ItemHeight, data->ld->ld_Font->tf_XSize, msg->ops_AttrList);
	   data->spacing    = GetTagData(ASLLV_Spacing, BORDERLVITEMSPACINGY * 2, msg->ops_AttrList);

	   data->lineheight = data->itemheight + data->spacing;

	   data->labels	    = (struct List *)GetTagData(ASLLV_Labels, NULL, msg->ops_AttrList);
	   NEWLIST(&data->emptylist);	   
	   if (!data->labels) data->labels = &data->emptylist;
	   
	   data->total = CountNodes(data->labels, 0);
	   data->visible = data->total;
	   data->active = -1;
	   data->rendersingleitem = -1;
	   
	   makenodetable(cl, o);
	   
	   data->renderhook = (struct Hook *)GetTagData(ASLLV_CallBack, NULL, msg->ops_AttrList);
    	   data->default_renderhook.h_Entry = (APTR) AROS_ASMSYMNAME(ASLLVRenderHook);
    	   data->default_renderhook.h_SubEntry = NULL;
    	   data->default_renderhook.h_Data = (APTR)AslBase;
	   if (!data->renderhook) data->renderhook = &data->default_renderhook;

	   data->domultiselect = GetTagData(ASLLV_DoMultiSelect, 0, msg->ops_AttrList) ? TRUE : FALSE;
	   
	   notifyall(cl, o, NULL, OPUF_INTERIM);
	}
    }

    return (IPTR)o;
}

/*************************
**  AslListView::Get()  **
*************************/
static IPTR asllistview_get(Class * cl, Object * o, struct opGet *msg)
{
    struct AslListViewData *data;
    IPTR retval = 1;
    
    data = INST_DATA(cl, o);
    
    switch(msg->opg_AttrID)
    {
        case ASLLV_Active:
	    *msg->opg_Storage = data->active;
	    break;
	
	case ASLLV_Top:
	    *msg->opg_Storage = data->top;
	    break;
	
	default:
	    retval = DoSuperMethodA(cl, o, (Msg)msg);
	    break;
	    
    } /* switch(msg->opg_AttrID) */
        
    return retval;
}

/*****************************
**  AslListView::Dispose()  **
*****************************/
static IPTR asllistview_dispose(Class * cl, Object * o, Msg msg)
{
    struct AslListViewData *data;
    IPTR retval;
    
    data = INST_DATA(cl, o);
    if (data->frame) DisposeObject(data->frame);
    if (data->nodetable) FreeVec(data->nodetable);
    retval = DoSuperMethodA(cl, o, msg);
    
    return retval;
}

/******************************
**  AslListView::GoActive()  **
******************************/
static IPTR asllistview_goactive(Class *cl, Object *o, struct gpInput *msg)
{
    struct AslListViewData *data;
    WORD i;
    
    IPTR retval = GMR_NOREUSE;
    
    data = INST_DATA(cl, o);
    i = mouseitem(cl, o, msg->gpi_Mouse.X, msg->gpi_Mouse.Y);
        
    if (i >= 0)
    {
        struct Node *node;
	
	if ((node = findnode(cl, o, i)))
	{
	    ULONG sec, micro;
	    
	    CurrentTime(&sec, &micro);
	    
	    if (data->domultiselect && IS_MULTISEL(node) &&
	    	(msg->gpi_IEvent->ie_Qualifier & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT)))
	    {
	        data->multiselecting = TRUE;
	    } else {
	        data->multiselecting = FALSE;
		
		if (data->domultiselect)
		{
		    struct Node *node;
		    WORD n = 0;
		    
		    ForeachNode(data->labels, node)
		    {
		        if (IS_MULTISEL(node) && IS_SELECTED(node) && (n != data->active))
			{
			    MARK_UNSELECTED(node);
			    rendersingleitem(cl, o, msg->gpi_GInfo, n);
			}
		        n++;
		    }
		}
	    }
	    
	    data->doubleclicked = FALSE;
	    if (data->active == i)
	    { 
		if (DoubleClick(data->clicksec, data->clickmicro, sec, micro))
		{
		    data->doubleclicked = TRUE;
		}
	    }
	    else
	    {		
		if (!data->multiselecting && (data->active >= 0))
		{
	            struct Node *oldnode = findnode(cl, o, data->active);

		    MARK_UNSELECTED(oldnode);
		    rendersingleitem(cl, o, msg->gpi_GInfo, data->active);
		}
		
		MARK_SELECTED(node);
		rendersingleitem(cl, o, msg->gpi_GInfo, i);
	        
	        data->active = i;
		
	    } /* if (data->active != i) */
	    
	    data->clicksec   = sec;
	    data->clickmicro = micro;
	    
	    retval = GMR_MEACTIVE;
	    
	} /* if ((node = findnode(cl, o, i))) */
	
    } /* if (i >= 0) */
	    
    return retval;
}

/*********************************
**  AslListView::HandleInput()  **
*********************************/
static IPTR asllistview_handleinput(Class *cl, Object *o, struct gpInput *msg)
{
    struct AslListViewData *data;
    IPTR retval = GMR_MEACTIVE;
    
    data = INST_DATA(cl, o);
    
    switch(msg->gpi_IEvent->ie_Class)
    {
        case IECLASS_RAWMOUSE:
	    switch(msg->gpi_IEvent->ie_Code)
	    {
	        case SELECTUP:
		    *msg->gpi_Termination = data->doubleclicked;
		    retval = GMR_VERIFY | GMR_NOREUSE;
		    break;
		    
		case IECODE_NOBUTTON: {	
		    WORD n = mouseitem(cl, o, msg->gpi_Mouse.X, msg->gpi_Mouse.Y);
		    
		    if ((n == -1) && (data->active > 0)) n = data->active - 1;
		    if ((n == -2) && (data->active < data->total - 1)) n = data->active + 1;
		    		    
		    if ((n >= 0) && (n != data->active))
		    {
		        struct Node *old = findnode(cl, o, data->active);
			struct Node *new = findnode(cl, o, n);
			
			if (data->multiselecting && new)
			{
			    if (!IS_MULTISEL(new)) new = NULL;
			}
			
			if (new && old)
			{
			    if (!data->multiselecting)
			    {
			        MARK_UNSELECTED(old);
			        rendersingleitem(cl, o, msg->gpi_GInfo, data->active);
			    }
			    
			    MARK_SELECTED(new);
			    rendersingleitem(cl, o, msg->gpi_GInfo, n);
			    
			    data->active = n;
			}

			if ((n < data->top) || (n >= data->top + data->visible))
			{
			    struct RastPort *rp;

			    if (n < data->top) data->top--; else data->top++;

			    if ((rp = ObtainGIRPort(msg->gpi_GInfo)))
			    {		    
				struct gpRender gpr;

				gpr.MethodID   = GM_RENDER;
				gpr.gpr_GInfo  = msg->gpi_GInfo;
				gpr.gpr_RPort  = rp;
				gpr.gpr_Redraw = GREDRAW_UPDATE;

				DoMethodA(o, (Msg)&gpr);

				ReleaseGIRPort(rp);
			    }

			    notifyall(cl, o, msg->gpi_GInfo, OPUF_INTERIM);
			    
			} /* if ((n < data->top) || (n >= data->top + data->visible)) */
		    
		    } /* if ((n >= 0) && (n != data->active)) */
		    break; }
		    
	    } /* switch(msg->gpi_IEvent->ie_Code) */
	    break;
	    
    } /* switch(msg->gpi_IEvent->ie_Class) */
    
    return retval;
}

/****************************
**  AslListView::Layout()  **
****************************/
static IPTR asllistview_layout(Class *cl, Object *o, struct gpLayout *msg)
{
    struct AslListViewData *data;
    IPTR retval = 0;
    
    data = INST_DATA(cl, o);
    
    if (msg->gpl_GInfo)
    {
        WORD newtop = data->top;
	WORD newvisible = data->visible;
	
	getgadgetcoords(G(o), msg->gpl_GInfo, &data->minx, &data->miny, &data->width, &data->height);

	data->maxx = data->minx + data->width  - 1;
	data->maxy = data->miny + data->height - 1;

	newvisible = (data->height - BORDERLVSPACINGY * 2) / data->lineheight;
	if (newtop + newvisible > data->total)
	{
	    newtop = data->total - newvisible;
	}
	if (newtop < 0) newtop = 0;
	
	if ((newtop != data->top) || (newvisible != data->visible) || (!data->layouted))
	{
	    data->top = newtop;
	    data->visible = newvisible;
	    
	    notifyall(cl, o, msg->gpl_GInfo, 0);
	}
		
	data->layouted = TRUE;
	
	retval = 1;
    }
    
    return retval;
}

/****************************
**  AslListView::Render()  **
****************************/
static IPTR asllistview_render(Class *cl, Object *o, struct gpRender *msg)
{
    struct AslListViewData *data;
    IPTR retval = 0;
    
    data = INST_DATA(cl, o);
    
    if (msg->gpr_Redraw == GREDRAW_REDRAW)
    {
        struct TagItem im_tags[] =
	{
	    {IA_Width	, data->width	},
	    {IA_Height	, data->height	},
	    {TAG_DONE			}
	};	

	SetAttrsA(data->frame, im_tags);
	
	DrawImageState(msg->gpr_RPort,
		       (struct Image *)data->frame,
		       data->minx,
		       data->miny,
		       IDS_NORMAL,
		       msg->gpr_GInfo->gi_DrInfo);

        renderallitems(cl, o, msg->gpr_RPort);

    } /* if (msg->gpr_Redraw == GREDRAW_REDRAW) */
    else if (msg->gpr_Redraw == GREDRAW_UPDATE)
    {
        if (data->rendersingleitem == -1)
	{
            renderallitems(cl, o, msg->gpr_RPort);
	} else {
	    if (data->rendersingleitem >= data->top)
	    {
	        struct Node *node = findnode(cl, o, data->rendersingleitem);
		
		renderitem(cl, o, node, data->rendersingleitem - data->top, msg->gpr_RPort);
	    }
	}
    }
    
    return retval;
}

AROS_UFH3S(IPTR, dispatch_asllistviewclass,
	  AROS_UFHA(Class *, cl, A0),
	  AROS_UFHA(Object *, obj, A2),
	  AROS_UFHA(Msg, msg, A1)
)
{
    IPTR retval = 0UL;

    switch (msg->MethodID)
    {
        case OM_NEW:
	    retval = asllistview_new(cl, obj, (struct opSet *)msg);
	    break;
	
	case OM_UPDATE:
	case OM_SET:
	    retval = asllistview_set(cl, obj, (struct opSet *)msg);
	    break;

	case OM_GET:
	    retval = asllistview_get(cl, obj, (struct opGet *)msg);
	    break;
	    
	case OM_DISPOSE:
	    retval = asllistview_dispose(cl, obj, msg);
	    break;
	
	case GM_GOACTIVE:
	    retval = asllistview_goactive(cl, obj , (struct gpInput *)msg);
	    break;
	    
	case GM_HANDLEINPUT:
	    retval = asllistview_handleinput(cl, obj , (struct gpInput *)msg);
	    break;
	    
	case GM_LAYOUT:
	    retval = asllistview_layout(cl, obj, (struct gpLayout *)msg);
	    break;
	    
	case GM_RENDER:
	    retval = asllistview_render(cl, obj, (struct gpRender *)msg);
	    break;
	    
	default:
	    retval = DoSuperMethodA(cl, obj, msg);
	    break;

    } /* switch (msg->MethodID) */
     
    return retval;
}

/********************** ASL BUTTON CLASS **************************************************/

struct AslButtonData
{
    Object 		*frame;
    struct LayoutData 	*ld;
    struct CoolImage  	*coolimage;
    ULONG		*coolimagepal;
};

/***********************
**  AslButton::New()  **
***********************/
static IPTR aslbutton_new(Class * cl, Object * o, struct opSet * msg)
{
    struct AslButtonData *data;
    struct TagItem fitags[] =
    {
	{IA_FrameType, FRAME_BUTTON},
	{IA_EdgesOnly, TRUE	   },
	{TAG_DONE, 0UL}
    };
    
    o = (Object *)DoSuperMethodA(cl, o, (Msg)msg);

    if (o)
    {
    	data = INST_DATA(cl, o);
	
	/* {GA_Image, whatever} means, a frame shall be created */

	if (FindTagItem(GA_Image, msg->ops_AttrList))
	{
	    if (G(o)->GadgetText) fitags[1].ti_Tag = TAG_IGNORE;
	    data->frame = NewObjectA(NULL, FRAMEICLASS, fitags);
	}
	
	data->coolimage = (struct CoolImage *)GetTagData(ASLBT_CoolImage, 0, msg->ops_AttrList);
	
	data->ld = (struct LayoutData *)GetTagData(GA_UserData, 0, msg->ops_AttrList);
	
	if (!data->ld)
	{
	    CoerceMethod(cl, o, OM_DISPOSE);
	    o = NULL;
	} else {
	    if (data->coolimage && data->ld->ld_TrueColor && CyberGfxBase)
	    {
	        WORD numcols = 1 << data->coolimage->depth;
		
	        if ((data->coolimagepal = AllocVec(numcols * sizeof(ULONG), MEMF_PUBLIC)))
		{
		    ULONG *p = data->coolimagepal;
		    WORD i;
		    
		    for(i = 0; i < numcols; i++)
		    {
		        *p++ = (data->coolimage->pal[i * 3] << 16) |
			       (data->coolimage->pal[i * 3 + 1] << 8) |
			       (data->coolimage->pal[i * 3 + 2]);
		    }
		    
		} else {
		    data->coolimage = NULL;
		}
	    } else {
	        data->coolimage = NULL;
	    }
	}
	
    } /* if (o) */

    return (IPTR)o;
}

/***************************
**  AslButton::Dispose()  **
***************************/
static IPTR aslbutton_dispose(Class * cl, Object * o, Msg msg)
{
    struct AslButtonData *data;
    IPTR retval;
    
    data = INST_DATA(cl, o);
    if (data->frame) DisposeObject(data->frame);
    if (data->coolimagepal) FreeVec(data->coolimagepal);
    
    retval = DoSuperMethodA(cl, o, msg);
    
    return retval;
}


/************************
**  AslButton::Render()  **
************************/
static IPTR aslbutton_render(Class *cl, Object *o, struct gpRender *msg)
{
    struct AslButtonData *data;
    char *text = (STRPTR)G(o)->GadgetText;
    struct TagItem im_tags[] =
    {
	{IA_Width	, 0	},
	{IA_Height	, 0	},
	{TAG_DONE		}
    };
    WORD x, y, w, h;

    getgadgetcoords(G(o), msg->gpr_GInfo, &x, &y, &w, &h);
    
    data = INST_DATA(cl, o);
    
    if (data->frame)
    {
	im_tags[0].ti_Data = w;
	im_tags[1].ti_Data = h;

	SetAttrsA(data->frame, im_tags);

	DrawImageState(msg->gpr_RPort,
		       (struct Image *)data->frame,
		       x,
		       y,
		       (!text || (G(o)->Flags & GFLG_SELECTED)) ? IDS_SELECTED : IDS_NORMAL,
		       msg->gpr_GInfo->gi_DrInfo);
    }
    
    if (text)
    {
	WORD len = strlen(text);
	WORD tx = x, ty = y;
	
	SetFont(msg->gpr_RPort, data->ld->ld_Font);

	if (data->coolimage)
	{
	    tx += BORDERIMAGESPACINGX + data->coolimage->width + BORDERIMAGESPACINGX;
	    w  -= (BORDERIMAGESPACINGX + data->coolimage->width + BORDERIMAGESPACINGX + BORDERIMAGESPACINGX);
	}
	
	tx += (w - TextLength(msg->gpr_RPort, text, len)) / 2; 
	ty += (h - msg->gpr_RPort->TxHeight) / 2 + msg->gpr_RPort->TxBaseline;
	
	SetABPenDrMd(msg->gpr_RPort,
		     data->ld->ld_Dri->dri_Pens[(G(o)->Flags & GFLG_SELECTED) ? FILLTEXTPEN : TEXTPEN],
		     0,
		     JAM1);
	Move(msg->gpr_RPort, tx, ty);
	Text(msg->gpr_RPort, text, len);
    } else {
    	x += 3; w -= 6;
	y += 3; h -= 6;
		 
	SetABPenDrMd(msg->gpr_RPort,
		     data->ld->ld_Dri->dri_Pens[(G(o)->Flags & GFLG_SELECTED) ? FILLPEN : BACKGROUNDPEN],
		     0,
		     JAM1);
        
	RectFill(msg->gpr_RPort, x, y, x + w - 1, y + h - 1);
    }

    if (data->coolimage)
    {
        ULONG bg = (G(o)->Flags & GFLG_SELECTED) ? 0x6688BB : 0xB3B3B3;
	
	data->coolimagepal[0] = bg;
	
	WriteLUTPixelArray((APTR)data->coolimage->data,
			    0,
			    0,
			    data->coolimage->width,
			    msg->gpr_RPort,
			    data->coolimagepal,
			    x + BORDERIMAGESPACINGX,
			    y + (h - data->coolimage->height) / 2,
			    data->coolimage->width,
			    data->coolimage->height,
			    CTABFMT_XRGB8);
        
    }
    	    
    return 0;
}

/**************************
**  AslButton::Layout()  **
**************************/
static IPTR aslbutton_layout(Class * cl, Object * o, struct gpLayout * msg)
{
    struct AslButtonData *data;
    IPTR retval;
    WORD innerwidth;
    WORD spacing;
    WORD x;
    
    data = INST_DATA(cl, o);
       
    retval = DoSuperMethodA(cl, o, (Msg)msg);
    
    switch (G(o)->GadgetID)
    {
        case ID_MAINBUTTON_OK:
	case ID_MAINBUTTON_MIDDLELEFT:
	case ID_MAINBUTTON_MIDDLERIGHT:
	case ID_MAINBUTTON_CANCEL:

	    /* multiply width 16 for sub-pixel accuracy */

 	    x = (data->ld->ld_WBorLeft + OUTERSPACINGX) * 16 + 8;
	    
    	    innerwidth = msg->gpl_GInfo->gi_Window->Width - 
	    		 msg->gpl_GInfo->gi_Window->BorderLeft -
			 msg->gpl_GInfo->gi_Window->BorderRight -
			 OUTERSPACINGX * 2;
	
	    /* multiply width 16 for sub-pixel accuracy */
	    	 
    	    spacing = (innerwidth - data->ld->ld_ButWidth * data->ld->ld_NumButtons) * 16 /
	    	      (data->ld->ld_NumButtons - 1);
 
	    x += (G(o)->GadgetID - ID_MAINBUTTON_OK) * (data->ld->ld_ButWidth * 16 + spacing);
	    G(o)->LeftEdge = x / 16;
	    break;
    }
   
    return retval;
}

AROS_UFH3S(IPTR, dispatch_aslbuttonclass,
	  AROS_UFHA(Class *, cl, A0),
	  AROS_UFHA(Object *, obj, A2),
	  AROS_UFHA(Msg, msg, A1)
)
{
    IPTR retval = 0UL;

    switch (msg->MethodID)
    {
        case OM_NEW:
	    retval = aslbutton_new(cl, obj, (struct opSet *)msg);
	    break;
	    
	case OM_DISPOSE:
	    retval = aslbutton_dispose(cl, obj, msg);
	    break;
	
	case GM_RENDER:
	    retval = aslbutton_render(cl, obj, (struct gpRender *)msg);
	    break;
	
	case GM_LAYOUT:
	    retval = aslbutton_layout(cl, obj, (struct gpLayout *)msg);
	    break;
	    
	default:
	    retval = DoSuperMethodA(cl, obj, msg);
	    break;

    } /* switch (msg->MethodID) */
    
    return retval;
}

/********************** ASL STRING CLASS **************************************************/

struct AslStringData
{
    Object *frame;
};

/***********************
**  AslString::New()  **
***********************/
static IPTR aslstring_new(Class * cl, Object * o, struct opSet * msg)
{
    struct AslStringData *data;
    struct TagItem fitags[] =
    {
	{IA_FrameType, FRAME_RIDGE},
	{TAG_DONE, 0UL}
    };
    
    o = (Object *)DoSuperMethodA(cl, o, (Msg)msg);
    if (o)
    {
    	data = INST_DATA(cl, o);

	EG(o)->BoundsLeftEdge = G(o)->LeftEdge;
	EG(o)->BoundsTopEdge  = G(o)->TopEdge;
	EG(o)->BoundsWidth    = G(o)->Width;
	EG(o)->BoundsHeight   = G(o)->Height;
	EG(o)->MoreFlags |= GMORE_BOUNDS;
	
	G(o)->LeftEdge += BORDERSTRINGSPACINGX;
	G(o)->TopEdge  += BORDERSTRINGSPACINGY;
	G(o)->Width    -= BORDERSTRINGSPACINGX * 2;
	G(o)->Height   -= BORDERSTRINGSPACINGY * 2;

	data->frame = NewObjectA(NULL, FRAMEICLASS, fitags);
	if (!data->frame)
	{
	    CoerceMethod(cl, o, OM_DISPOSE);
	    o = NULL;
	}
    }

    return (IPTR)o;
}

/***************************
**  AslString::Dispose()  **
***************************/
static IPTR aslstring_dispose(Class * cl, Object * o, Msg msg)
{
    struct AslStringData *data;
    IPTR retval;
    
    data = INST_DATA(cl, o);
    if (data->frame) DisposeObject(data->frame);
    
    retval = DoSuperMethodA(cl, o, msg);
    
    return retval;
}

/**************************
**  AslString::Render()  **
**************************/
static IPTR aslstring_render(Class *cl, Object *o, struct gpRender *msg)
{
    struct AslStringData *data;
    IPTR retval;
    
    data = INST_DATA(cl, o);
    
    if (msg->gpr_Redraw == GREDRAW_REDRAW)
    {
        struct TagItem im_tags[] =
	{
	    {IA_Width	, 0	},
	    {IA_Height	, 0	},
	    {TAG_DONE		}
	};	
	WORD x, y, w, h;
	
	getgadgetcoords(G(o), msg->gpr_GInfo, &x, &y, &w, &h);
	
	x -= BORDERSTRINGSPACINGX;
	y -= BORDERSTRINGSPACINGY;
	w += BORDERSTRINGSPACINGX * 2;
	h += BORDERSTRINGSPACINGY * 2;
	
	im_tags[0].ti_Data = w;
	im_tags[1].ti_Data = h;
	
	SetAttrsA(data->frame, im_tags);
	
	DrawImageState(msg->gpr_RPort,
		       (struct Image *)data->frame,
		       x,
		       y,
		       IDS_NORMAL,
		       msg->gpr_GInfo->gi_DrInfo);

    } /* if (msg->gpr_Redraw == GREDRAW_REDRAW) */
    
    retval = DoSuperMethodA(cl, o, (Msg)msg);
    
    return retval;
}

AROS_UFH3S(IPTR, dispatch_aslstringclass,
	  AROS_UFHA(Class *, cl, A0),
	  AROS_UFHA(Object *, obj, A2),
	  AROS_UFHA(Msg, msg, A1)
)
{
    IPTR retval = 0UL;

    switch (msg->MethodID)
    {
        case OM_NEW:
	    retval = aslstring_new(cl, obj, (struct opSet *)msg);
	    break;
	
	case OM_DISPOSE:
	    retval = aslstring_dispose(cl, obj, msg);
	    break;
	    
	case GM_RENDER:
	    retval = aslstring_render(cl, obj, (struct gpRender *)msg);
	    break;
	    
	default:
	    retval = DoSuperMethodA(cl, obj, msg);
	    break;

    } /* switch (msg->MethodID) */
    
    return retval;
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
#undef AslBase

Class *makeaslpropclass(struct AslBase_intern * AslBase)
{
    Class *cl = NULL;

    if (AslBase->aslpropclass)
	return AslBase->aslpropclass;

    cl = MakeClass(NULL, PROPGCLASS, NULL, sizeof(struct AslPropData), 0UL);
    if (!cl)
	return NULL;
	
    cl->cl_Dispatcher.h_Entry = (APTR) AROS_ASMSYMNAME(dispatch_aslpropclass);
    cl->cl_Dispatcher.h_SubEntry = NULL;
    cl->cl_UserData = (IPTR) AslBase;

    AslBase->aslpropclass = cl;

    return cl;
}

/***********************************************************************************/

Class *makeaslarrowclass(struct AslBase_intern * AslBase)
{
    Class *cl = NULL;

    if (AslBase->aslarrowclass)
	return AslBase->aslarrowclass;

    cl = MakeClass(NULL, BUTTONGCLASS, NULL, sizeof(struct AslArrowData), 0UL);
    if (!cl)
	return NULL;
	
    cl->cl_Dispatcher.h_Entry = (APTR) AROS_ASMSYMNAME(dispatch_aslarrowclass);
    cl->cl_Dispatcher.h_SubEntry = NULL;
    cl->cl_UserData = (IPTR) AslBase;

    AslBase->aslarrowclass = cl;

    return cl;
}

/***********************************************************************************/

Class *makeasllistviewclass(struct AslBase_intern * AslBase)
{
    Class *cl = NULL;

    if (AslBase->asllistviewclass)
	return AslBase->asllistviewclass;

    cl = MakeClass(NULL, GADGETCLASS, NULL, sizeof(struct AslListViewData), 0UL);
    if (!cl)
	return NULL;
	
    cl->cl_Dispatcher.h_Entry = (APTR) AROS_ASMSYMNAME(dispatch_asllistviewclass);
    cl->cl_Dispatcher.h_SubEntry = NULL;
    cl->cl_UserData = (IPTR) AslBase;

    AslBase->asllistviewclass = cl;

    return cl;
}

/***********************************************************************************/

Class *makeaslbuttonclass(struct AslBase_intern * AslBase)
{
    Class *cl = NULL;

    if (AslBase->aslbuttonclass)
	return AslBase->aslbuttonclass;

   cl = MakeClass(NULL, BUTTONGCLASS, NULL, sizeof(struct AslButtonData), 0UL);
    if (!cl)
	return NULL;
	
    cl->cl_Dispatcher.h_Entry = (APTR) AROS_ASMSYMNAME(dispatch_aslbuttonclass);
    cl->cl_Dispatcher.h_SubEntry = NULL;
    cl->cl_UserData = (IPTR) AslBase;

    AslBase->aslbuttonclass = cl;

    return cl;
}

/***********************************************************************************/

Class *makeaslstringclass(struct AslBase_intern * AslBase)
{
    Class *cl = NULL;

    if (AslBase->aslstringclass)
	return AslBase->aslstringclass;

   cl = MakeClass(NULL, STRGCLASS, NULL, sizeof(struct AslStringData), 0UL);
    if (!cl)
	return NULL;
	
    cl->cl_Dispatcher.h_Entry = (APTR) AROS_ASMSYMNAME(dispatch_aslstringclass);
    cl->cl_Dispatcher.h_SubEntry = NULL;
    cl->cl_UserData = (IPTR) AslBase;

    AslBase->aslstringclass = cl;

    return cl;
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
