#include "profile_selector.hpp"

ProfileSelectorWidget::ProfileSelectorWidget(ProcWatch &pw, Config &cfg, NVIDIA &nv) noexcept:
        pw(pw),
        cfg(cfg),
        nv(nv),
        dvc_map(nv.getVibrance())
{
        // build profile tabs from Config
        createProfileTabsBox();

        // build profile selector buttons area
        createProfileSelectorButtonBox();

        profileSelectorWidgetLayout.addWidget(&tabs);
        profileSelectorWidgetLayout.addWidget(&profileSelectorBox);
        setLayout(&profileSelectorWidgetLayout);
}

auto ProfileSelectorWidget::createProfileTabsBox() noexcept -> void
{
        auto config = cfg.queryProfiles();
        // make sure that CONFIG_DEFAULT_PROFILE_STR is on top of the list
        config.swap(0,config.indexOf(CONFIG_DEFAULT_PROFILE_STR));

        for(const auto &app : config)
        {
                // FIXME: use icon if available, label otherwise (_not_ both)
                // for now, just ignore icons

                // QIcon icon(cfg.queryIconPath(app));
                // tabs.addTab(new AppProfile(app, this), icon, app);
                tabs.addTab(new AppProfile(app, this), app);
        }
        tabs.setTabsClosable(true);
        // hide close icon for default profile
        tabs.tabBar()->tabButton(0,QTabBar::RightSide)->hide();
        tabs.setUsesScrollButtons(false);

        tabs.setIconSize(QSize(0,0));
        tabs.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        connect(&tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(removeProfile(int)));
}

auto ProfileSelectorWidget::removeProfile(int index) noexcept -> void
{
        auto name = tabs.tabText(index);

        cfg.remove(name);
        pw.removeRule(name.toStdString());
        tabs.removeTab(index);
}

auto ProfileSelectorWidget::createProfileSelectorButtonBox() noexcept -> void
{
        profileSelectorBox.setStyleSheet("border:none");

        updateComboBox(0);
        connect(&profileList, SIGNAL(activated(int)), this, SLOT(updateComboBox(int)));

        profileSelectorLayout.addWidget(&profileList);
        profileSelectorBox.setLayout(&profileSelectorLayout);
}

// adds new profile from create profile list widget, afterwards fils it with {current processes } - {profile list}
auto ProfileSelectorWidget::updateComboBox(int index) noexcept -> void
{
        QStringList qlist;
        qlist << "Add new profile";
        for(auto e : pw.listRunningProcs())
                qlist << QString(e.c_str());

        // if we selected some new profile name from list
        if(index)
        {
                QString name(profileList.itemText(index));
                // TODO: dynamically add icon instead of using this default
                QIcon icon(":/resources/xclient.svg");
                cfg.setDVC(name, dvc_map);
                cfg.setIconPath(name, ":/resources/xclient.svg");
                tabs.addTab(new AppProfile(name, this), icon, name);
        }

        // remove items that are already in cfg from the list of potential new app profiles
        for(auto &e : cfg.queryProfiles())
        {
                qlist.removeOne(QString(e));
        }

        profileList.clear();
        profileList.addItems(qlist);
}

auto ProfileSelectorWidget::applyDVC() noexcept -> void
{
        // FIXME: get rid of this
        std::map<int,int> std_map = dvc_map.toStdMap();
        nv.setVibrance(std_map);
}

AppProfile::AppProfile(const QString name, ProfileSelectorWidget *p) noexcept :
        PSW(p),        // pointer to master PSW (has NV object)
        name(name) // profile name (or CONFIG_DEFAULT_PROFILE_STR)
{
        // get DVC map from Config file
        auto dvc_map = PSW->cfg.queryDVC(name);
        // If we can't find it there, create it based on current config
        if(dvc_map.empty())
        {
                dvc_map = DVC_map(PSW->nv.getVibrance());
                PSW->cfg.setDVC(name, dvc_map);
        }

        // add new rule to procWatch
        auto name_std = name.toStdString();
        auto map_std = dvc_map.toStdMap();
        PSW->pw.updateRule(name_std, map_std);

        // create one DVCentry per each CRTC (dpyId)
        DVC_map::const_iterator i = dvc_map.constBegin();
        while(i != dvc_map.constEnd())
        {
                vlayout.addWidget(new DVCEntry(i.key(), dvc_map[i.key()], this));
                ++i;
        }

        setLayout(&vlayout);
        setStyleSheet("border:none");
}

DVCEntry::DVCEntry(int dpy_id, int dvc_level, AppProfile *p) noexcept:
        dpy_id(dpy_id),        // monitor id
        dvc(dvc_level),        // reference to NVIDIA std::map
        dvc_slider(Qt::Horizontal),
        dpy_name("DPY-" + QString::number(dpy_id)),
        AP(p)        // reference to parent AppProfile widget
{
        dvc_slider.setMinimum(-100);
        dvc_slider.setMaximum(100);
        dvc_slider.setValue(dvc);

        dvc_le.setText(QString::number(dvc));
        dvc_le.setReadOnly(true);
        dvc_le.setMaximumWidth(30);

        connect(&dvc_slider,
                SIGNAL(valueChanged(int)),
                this,
                SLOT(onDVCSliderChanged(int)));

        hlayout.addWidget(&dpy_name);
        hlayout.addWidget(&dvc_slider);
        hlayout.addWidget(&dvc_le);
        setLayout(&hlayout);
}

auto DVCEntry::onDVCSliderChanged(int value) noexcept -> void
{
        dvc_le.setText(QString::number(dvc_slider.sliderPosition()));
        dvc = value;

        // if we have default tab, apply values immediatelly
        if(AP->name.compare(CONFIG_DEFAULT_PROFILE_STR) == 0)
        {
                AP->PSW->dvc_map[dpy_id] = value;
                AP->PSW->applyDVC();
        }

        // update config
        auto cfg_dvc_map = AP->PSW->cfg.queryDVC(AP->name);
        cfg_dvc_map[dpy_id] = value;
        // update config
        AP->PSW->cfg.setDVC(AP->name, cfg_dvc_map);
        // update procWatch rule
        auto name = AP->name.toStdString();
        auto map = cfg_dvc_map.toStdMap();
        AP->PSW->pw.updateRule(name, map);
}
