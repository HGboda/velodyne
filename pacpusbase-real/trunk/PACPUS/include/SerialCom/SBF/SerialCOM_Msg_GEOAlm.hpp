/*!
\class SerialCOM_Msg_GEOAlm
\brief Decoded almanach data for SBAS SV
\author C. Fouque
\version 1.0
\date Dec. 2008
\bug None
\warning None

This block contains the decoded almanac data for one SBAS satellite, as transmitted in SBAS message type 17. Refer to section A.4.4.12 of DO 229 [2] for further details.

<b>Output rate:</b> if enabled, this block is generated at least each time MT17 is received from an SBAS satellite. A different GEOAlm block is generated for each of the up to three almanac data sets in MT17. Additionally, each time the Alm category is specified in the SetSBFOutput command, all the almanac parameters currently available in the receiver are output.

 */

#pragma warning( once : 4290 )
#pragma warning( once : 4274 )

#ifndef _SerialCOM_Msg_GEOAlm_Plrx_H
#define _SerialCOM_Msg_GEOAlm_Plrx_H

#include <vector>
#include "SerialCOM_Msg.hpp"
#include "SerialCOM_Protocol.hpp"
#include "structure_septentrio.h"

namespace Plrx
{

   class SerialCOM_Msg_GEOAlm : public SerialCOM_Msg , public SbfDataGEOAlm
   {

      public :

         /*! \brief Constructor. Intitialise the member variables*/
         SerialCOM_Msg_GEOAlm();

         /*! \brief Copy constructor*/
	//SerialCOM_Msg_RXM_SBAS (const SerialCOM_Msg_RXM_SBAS &source);

	/*! \brief reset internal variables of the message after an error or the end of a
         *  message */
         /*! For more information see SerialCOM_Msg::reset*/
         void reset();

	/*! \brief Read the core of the message to check if the object is able to handle this
         *  message*/
         /*! For more information see SerialCOM_Msg::identify*/
         int identify(vector<unsigned char> *pbuffer);

         /*! \brief extracts the data of the message contained in the buffer vector*/
         /*! For more information see SerialCOM_Msg::formatData*/
         int formatData(vector<unsigned char> *pbuffer);

         int fromStructFile(void *buf);

	//! return the size of the structure SbfDataGEOAlm
         int getSizeOfStruct(){return sizeof(SbfDataGEOAlm);};

	//! write the Data of the message in the DByte file
         virtual int writeDByte(road_time_t currentRoadtime, road_timerange_t currentTimerange);

         //! Display Data to stdout;
         virtual void stdoutDisplay();

   };

};


#endif