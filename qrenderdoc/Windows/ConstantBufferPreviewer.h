/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#pragma once

#include <QFrame>

namespace Ui
{
class ConstantBufferPreviewer;
}

class RDTreeWidgetItem;
struct FormatElement;

class ConstantBufferPreviewer : public QFrame, public IConstantBufferPreviewer, public ICaptureViewer
{
  Q_OBJECT

public:
  explicit ConstantBufferPreviewer(ICaptureContext &ctx, const ShaderStage stage, uint32_t slot,
                                   uint32_t idx, QWidget *parent = 0);
  ~ConstantBufferPreviewer();

  static ConstantBufferPreviewer *has(ShaderStage stage, uint32_t slot, uint32_t idx);
  static ConstantBufferPreviewer *getOne();

  // IConstantBufferPreviewer
  QWidget *Widget() override { return this; }
  // ICaptureViewer
  void OnCaptureLoaded() override;
  void OnCaptureClosed() override;
  void OnSelectedEventChanged(uint32_t eventID) override {}
  void OnEventChanged(uint32_t eventID) override;

private slots:
  // automatic slots
  void on_setFormat_toggled(bool checked);
  void on_saveCSV_clicked();
  void on_resourceDetails_clicked();

  // manual slots
  void processFormat(const QString &format);

private:
  Ui::ConstantBufferPreviewer *ui;
  ICaptureContext &m_Ctx;

  ResourceId m_cbuffer;
  ResourceId m_shader;
  ShaderStage m_stage = ShaderStage::Vertex;
  uint32_t m_slot = 0;
  uint32_t m_arrayIdx = 0;

  void exportCSV(QTextStream &ts, const QString &prefix, RDTreeWidgetItem *item);

  rdcarray<ShaderVariable> applyFormatOverride(const bytebuf &data);

  void addVariables(RDTreeWidgetItem *root, const rdcarray<ShaderVariable> &vars);
  void setVariables(const rdcarray<ShaderVariable> &vars);

  rdcarray<ShaderVariable> m_Vars;

  bool updateVariables(RDTreeWidgetItem *root, const rdcarray<ShaderVariable> &prevVars,
                       const rdcarray<ShaderVariable> &newVars);

  void updateLabels();

  static QList<ConstantBufferPreviewer *> m_Previews;

  QList<FormatElement> m_formatOverride;
};
