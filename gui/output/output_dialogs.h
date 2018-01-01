// ************************************************************************** //
//
//  Steca2: stress and texture calculator
//
//! @file      gui/output/output_dialogs.h
//! @brief     Defines ...
//!
//! @homepage  https://github.com/scgmlz/Steca2
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2017
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
// ************************************************************************** //

#ifndef OUTPUT_DIALOGS_H
#define OUTPUT_DIALOGS_H

#include "calc/calc_reflection_info.h"
#include "def/special_pointers.h"
#include "panels/panel.h"

class QProgressBar;

namespace gui {

class Action;

namespace output {

class Params : public QWidget {
public:
    enum ePanels {
        REFLECTION = 0x01,
        GAMMA = 0x02,
        POINTS = 0x04,
        INTERPOLATION = 0x08,
        DIAGRAM = 0x10,
    };
    Params(ePanels);
    ~Params();
    class PanelReflection* panelReflection;
    class PanelGammaSlices* panelGammaSlices;
    class PanelGammaRange* panelGammaRange;
    class PanelPoints* panelPoints;
    class PanelInterpolation* panelInterpolation;
    class PanelDiagram* panelDiagram;
    str saveDir, saveFmt;
    void readSettings();
    void saveSettings() const;
    QBoxLayout* box_;
};


class Table : public TreeView {
public:
    Table(uint numDataColumns);
    void setColumns(QStringList const& headers, QStringList const& outHeaders, typ::cmp_vec const&);
    QStringList const outHeaders() { return outHeaders_; }
    void clear();
    void addRow(typ::row_t const&, bool sort);
    void sortData();
    uint rowCount() const;
    typ::row_t const& row(uint) const;
    scoped<class TableModel*> model_;
    QStringList outHeaders_;
};


class Tabs : public panel::TabsPanel {
private:
    using super = panel::TabsPanel;
};


class Tab : public QWidget {
public :
    Tab(Params&);
protected:
    Params& params_;
    GridLayout* grid_;
};


class TabTable : public Tab {
private:
    using super = Tab;
public:
    TabTable(Params&, QStringList const& headers, QStringList const& outHeaders,
             typ::cmp_vec const&);
private:
    struct showcol_t {
        str name;
        QCheckBox* cb;
    };
    typedef typ::vec<showcol_t> showcol_vec;

    class ShowColsWidget : public QWidget {
    private:
    public:
        ShowColsWidget(Table&, showcol_vec&);
    private:
        Table& table_;
        showcol_vec& showCols_;
        QBoxLayout* box_;
        QRadioButton *rbHidden_, *rbAll_, *rbNone_, *rbInten_, *rbTth_, *rbFWHM_;
    };
public:
    Table* table;
private:
    ShowColsWidget* showColumnsWidget_;
    showcol_vec showCols_;
};


class TabSave : public Tab {
private:
    using super = Tab;
public:
    TabSave(Params&, bool withTypes);
    str filePath(bool withSuffix);
    str separator() const;
    QAction *actBrowse, *actSave;
protected:
    str fileSetSuffix(rcstr);
    QLineEdit *dir_, *file_;
    QRadioButton *rbDat_, *rbCsv_;
};


class Frame : public QFrame {
private:
    using super = QFrame;
public:
    Frame(rcstr title, Params*, QWidget*);
protected:
    QAction *actClose_, *actCalculate_, *actInterpolate_;
    QToolButton *btnClose_, *btnCalculate_, *btnInterpolate_;
    QProgressBar* pb_;
    QBoxLayout* box_;
    Params* params_;
    Tabs* tabs_;
    typ::vec<calc::ReflectionInfos> calcPoints_, interpPoints_;
    Table* table_;
    void calculate();
    void interpolate();
    virtual void displayReflection(uint reflIndex, bool interpolated);
    uint getReflIndex() const;
    bool getInterpolated() const;
};

} // namespace output
} // namespace gui

#endif // OUTPUT_DIALOGS_H
