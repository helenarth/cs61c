//
//  MenuBarBuilder.java
//  Proj1_1
//
//  Created by Nathan brummel on 9/20/12.
//  Copyright (c) 2012 Berkeley. All rights reserved.
//

package com.example.app;

import javax.swing.*;

/**
 * Creates a consistant menu bar experience across both windows.
 * 
 * Since the only differentiation between this sample's two windows is the close
 * action, this one class can be responsible for assembling the entire menu bar.
 * 
 * Certain menu items are not added on the Mac because they are expected to be
 * handled by the OSXAppAdapter installing an EAWT ApplicationListener.
 * 
 * @author Sabersimon
 */
public class MenuBarBuilder {
	public static JMenuBar createMenuBar(final ApplicationController app, final Action closeAction) {
		final JMenuBar menuBar = new JMenuBar();
		
		menuBar.add(createFileMenu(app, closeAction));
		menuBar.add(createEditMenu(app));
		menuBar.add(createHelpMenu(app));
		
		return menuBar;
	}
	
	static JMenu createFileMenu(final ApplicationController app, final Action closeAction) {
		final JMenu menu = new JMenu(app.getLocalizer().getText("app.file.menu", "File"));
		
		menu.add(new JMenuItem(app.getActions().openFile));
		menu.add(new JMenuItem(closeAction));
		if (!app.getLocalizer().isMac) {
			menu.addSeparator();
			menu.add(new JMenuItem(app.getActions().quit));
		}
		
		return menu;
	}
	
	static JMenu createEditMenu(final ApplicationController app) {
		final JMenu menu = new JMenu(app.getLocalizer().getText("app.edit.menu", "Edit"));
		
		menu.add(new JMenuItem(app.getActions().cut));
		menu.add(new JMenuItem(app.getActions().copy));
		menu.add(new JMenuItem(app.getActions().paste));
		menu.add(new JMenuItem(app.getActions().selectAll));
		if (!app.getLocalizer().isMac) {
			menu.addSeparator();
			menu.add(new JMenuItem(app.getActions().preferences));
		}
		
		return menu;
	}
	
	static JMenu createHelpMenu(final ApplicationController app) {
		final JMenu menu = new JMenu(app.getLocalizer().getText("app.help.menu", "Help"));
		
		if (!app.getLocalizer().isMac) {
			menu.add(new JMenuItem(app.getActions().about));
			menu.addSeparator();
		}
		menu.add(new JMenuItem(app.getActions().help));
		
		return menu;
	}
}
