/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "MontageFileListInfo.h"

// -----------------------------------------------------------------------------
MontageFileListInfo::MontageFileListInfo() = default;

// -----------------------------------------------------------------------------
MontageFileListInfo::~MontageFileListInfo() = default;

// -----------------------------------------------------------------------------
MontageFileListInfo::MontageFileListInfo(int32_t rowStart, int32_t rowEnd, int32_t colStart, int32_t colEnd)
: RowStart(rowStart)
, RowEnd(rowEnd)
, ColStart(colStart)
, ColEnd(colEnd)
{
}

// -----------------------------------------------------------------------------
void MontageFileListInfo::writeJson(QJsonObject& json) const
{
  writeSuperclassJson(json);

  json["RowStart"] = static_cast<qint32>(RowStart);
  json["RowEnd"] = static_cast<qint32>(RowEnd);
  json["ColStart"] = static_cast<qint32>(ColStart);
  json["ColEnd"] = static_cast<qint32>(ColEnd);
}

// -----------------------------------------------------------------------------
bool MontageFileListInfo::readJson(QJsonObject& json)
{
  bool result = readSuperclassJson(json);
  if(!result)
  {
    return false;
  }

  if(json["RowStart"].isDouble() && json["RowEnd"].isDouble() && json["ColStart"].isDouble() && json["ColEnd"].isDouble())
  {
    RowStart = static_cast<qint32>(json["RowStart"].toInt());
    RowEnd = static_cast<quint32>(json["RowEnd"].toInt());
    ColStart = static_cast<qint32>(json["ColStart"].toInt());
    ColEnd = static_cast<qint32>(json["ColEnd"].toInt());
    return true;
  }
  return false;
}
