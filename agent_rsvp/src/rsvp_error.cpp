/***************************************************************************
 *   This is an implementation of OIF UNI-C RSVP and IETF UNI-C RSVP.      *
 *   The code was created for purpose of EC IST Mupbed project.            *
 *   Copyright (C) 2007 by Poznan Supercomputing and Networking Center     *
 *   http://www.man.poznan.pl                                              *
 *                                                                         *     
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
#ifndef RSVP_ERROR_BODY_FILE
#define RSVP_ERROR_BODY_FILE

#include "rsvp_error.h"

char* error_texts[256] =
{
    /*000*/ "No error",
    /*001*/ "Incorrect parameter",
    /*002*/ "Cannot allocate memory (malloc error)",
    /*003*/ "String error (strlen gives negative value)",
    /*004*/ "Value not present",
    /*005*/ "Value already present",
    /*006*/ "Feature not implemented",
    /*007*/ "Socket creation error",
    /*008*/ "Socket not functional",
    /*009*/ "Packet sending error",
    /*010*/ "Timeout",
    /*011*/ "Select() error",
    /*012*/ "Receive buffer is full",
    /*013*/ "Receive buffer is empty",
    /*014*/ "Buffer too small",
    /*015*/ "Socket closing error",
    /*016*/ "Cannot add object: configuration insufficient",
    /*017*/ "Too many objects in packet",
    /*018*/ "Differing void element sizes",
    /*019*/ "Packet size is not a multiple of 4",
    /*020*/ "Maximum number of elements reached",
    /*021*/ "Checksum is incorrect",
    /*022*/ "Cannot destroy path",
    /*023*/ "Semaphore operation interrupted by signal",
    /*024*/ "No registered interface",
    /*025*/ "No registered connection with such ID",
    /*026*/ "Interface is not usable (e.g. not connected)",
    /*027*/ "Undefined",
    /*028*/ "Undefined",
    /*029*/ "Undefined",
    /*030*/ "Undefined",
    /*031*/ "Undefined",
    /*032*/ "Undefined",
    /*033*/ "Undefined",
    /*034*/ "Undefined",
    /*035*/ "Undefined",
    /*036*/ "Undefined",
    /*037*/ "Undefined",
    /*038*/ "Undefined",
    /*039*/ "Undefined",
    /*040*/ "Undefined",
    /*041*/ "Undefined",
    /*042*/ "Undefined",
    /*043*/ "Undefined",
    /*044*/ "Undefined",
    /*045*/ "Undefined",
    /*046*/ "Undefined",
    /*047*/ "Undefined",
    /*048*/ "Undefined",
    /*049*/ "Undefined",
    /*050*/ "Undefined",
    /*051*/ "Undefined",
    /*052*/ "Undefined",
    /*053*/ "Undefined",
    /*054*/ "Undefined",
    /*055*/ "Undefined",
    /*056*/ "Undefined",
    /*057*/ "Undefined",
    /*058*/ "Undefined",
    /*059*/ "Undefined",
    /*060*/ "Undefined",
    /*061*/ "Undefined",
    /*062*/ "Undefined",
    /*063*/ "Undefined",
    /*064*/ "Undefined",
    /*065*/ "Undefined",
    /*066*/ "Undefined",
    /*067*/ "Undefined",
    /*068*/ "Undefined",
    /*069*/ "Undefined",
    /*070*/ "Undefined",
    /*071*/ "Undefined",
    /*072*/ "Undefined",
    /*073*/ "Undefined",
    /*074*/ "Undefined",
    /*075*/ "Undefined",
    /*076*/ "Undefined",
    /*077*/ "Undefined",
    /*078*/ "Undefined",
    /*079*/ "Undefined",
    /*080*/ "Undefined",
    /*081*/ "Undefined",
    /*082*/ "Undefined",
    /*083*/ "Undefined",
    /*084*/ "Undefined",
    /*085*/ "Undefined",
    /*086*/ "Undefined",
    /*087*/ "Undefined",
    /*088*/ "Undefined",
    /*089*/ "Undefined",
    /*090*/ "Undefined",
    /*091*/ "Undefined",
    /*092*/ "Undefined",
    /*093*/ "Undefined",
    /*094*/ "Undefined",
    /*095*/ "Undefined",
    /*096*/ "Undefined",
    /*097*/ "Undefined",
    /*098*/ "Undefined",
    /*099*/ "Undefined",
    /*100*/ "Undefined",
    /*101*/ "Undefined",
    /*102*/ "Undefined",
    /*103*/ "Undefined",
    /*104*/ "Undefined",
    /*105*/ "Undefined",
    /*106*/ "Undefined",
    /*107*/ "Undefined",
    /*108*/ "Undefined",
    /*109*/ "Undefined",
    /*110*/ "Undefined",
    /*111*/ "Undefined",
    /*112*/ "Undefined",
    /*113*/ "Undefined",
    /*114*/ "Undefined",
    /*115*/ "Undefined",
    /*116*/ "Undefined",
    /*117*/ "Undefined",
    /*118*/ "Undefined",
    /*119*/ "Undefined",
    /*120*/ "Undefined",
    /*121*/ "Undefined",
    /*122*/ "Undefined",
    /*123*/ "Undefined",
    /*124*/ "Undefined",
    /*125*/ "Undefined",
    /*126*/ "Undefined",
    /*127*/ "Undefined",
    /*128*/ "Undefined",
    /*129*/ "Undefined",
    /*130*/ "Undefined",
    /*131*/ "Undefined",
    /*132*/ "Undefined",
    /*133*/ "Undefined",
    /*134*/ "Undefined",
    /*135*/ "Undefined",
    /*136*/ "Undefined",
    /*137*/ "Undefined",
    /*138*/ "Undefined",
    /*139*/ "Undefined",
    /*140*/ "Undefined",
    /*141*/ "Undefined",
    /*142*/ "Undefined",
    /*143*/ "Undefined",
    /*144*/ "Undefined",
    /*145*/ "Undefined",
    /*146*/ "Undefined",
    /*147*/ "Undefined",
    /*148*/ "Undefined",
    /*149*/ "Undefined",
    /*150*/ "Undefined",
    /*151*/ "Undefined",
    /*152*/ "Undefined",
    /*153*/ "Undefined",
    /*154*/ "Undefined",
    /*155*/ "Undefined",
    /*156*/ "Undefined",
    /*157*/ "Undefined",
    /*158*/ "Undefined",
    /*159*/ "Undefined",
    /*160*/ "Undefined",
    /*161*/ "Undefined",
    /*162*/ "Undefined",
    /*163*/ "Undefined",
    /*164*/ "Undefined",
    /*165*/ "Undefined",
    /*166*/ "Undefined",
    /*167*/ "Undefined",
    /*168*/ "Undefined",
    /*169*/ "Undefined",
    /*170*/ "Undefined",
    /*171*/ "Undefined",
    /*172*/ "Undefined",
    /*173*/ "Undefined",
    /*174*/ "Undefined",
    /*175*/ "Undefined",
    /*176*/ "Undefined",
    /*177*/ "Undefined",
    /*178*/ "Undefined",
    /*179*/ "Undefined",
    /*180*/ "Undefined",
    /*181*/ "Undefined",
    /*182*/ "Undefined",
    /*183*/ "Undefined",
    /*184*/ "Undefined",
    /*185*/ "Undefined",
    /*186*/ "Undefined",
    /*187*/ "Undefined",
    /*188*/ "Undefined",
    /*189*/ "Undefined",
    /*190*/ "Undefined",
    /*191*/ "Undefined",
    /*192*/ "Undefined",
    /*193*/ "Undefined",
    /*194*/ "Undefined",
    /*195*/ "Undefined",
    /*196*/ "Undefined",
    /*197*/ "Undefined",
    /*198*/ "Undefined",
    /*199*/ "Undefined",
    /*200*/ "Undefined",
    /*201*/ "Undefined",
    /*202*/ "Undefined",
    /*203*/ "Undefined",
    /*204*/ "Undefined",
    /*205*/ "Undefined",
    /*206*/ "Undefined",
    /*207*/ "Undefined",
    /*208*/ "Undefined",
    /*209*/ "Undefined",
    /*210*/ "Undefined",
    /*211*/ "Undefined",
    /*212*/ "Undefined",
    /*213*/ "Undefined",
    /*214*/ "Undefined",
    /*215*/ "Undefined",
    /*216*/ "Undefined",
    /*217*/ "Undefined",
    /*218*/ "Undefined",
    /*219*/ "Undefined",
    /*220*/ "Undefined",
    /*221*/ "Undefined",
    /*222*/ "Undefined",
    /*223*/ "Undefined",
    /*224*/ "Undefined",
    /*225*/ "Undefined",
    /*226*/ "Undefined",
    /*227*/ "Undefined",
    /*228*/ "Undefined",
    /*229*/ "Undefined",
    /*230*/ "Undefined",
    /*231*/ "Undefined",
    /*232*/ "Undefined",
    /*233*/ "Undefined",
    /*234*/ "Undefined",
    /*235*/ "Undefined",
    /*236*/ "Undefined",
    /*237*/ "Undefined",
    /*238*/ "Undefined",
    /*239*/ "Undefined",
    /*240*/ "Undefined",
    /*241*/ "Undefined",
    /*242*/ "Undefined",
    /*243*/ "Undefined",
    /*244*/ "Undefined",
    /*245*/ "Undefined",
    /*246*/ "Undefined",
    /*247*/ "Undefined",
    /*248*/ "Undefined",
    /*249*/ "Undefined",
    /*250*/ "Undefined",
    /*251*/ "Undefined",
    /*252*/ "Undefined",
    /*253*/ "Undefined",
    /*254*/ "Undefined",
    /*255*/ "Undefined"
};

char* decode_error(int errnum)
{
    return strdup(error_texts[ ((errnum<0)?-errnum:errnum)>255?255:((errnum<0)?-errnum:errnum) ]);
}

#endif
