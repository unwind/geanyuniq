Geanyuniq
=========
Geanyuniq is a plugin for the [Geany](http://geany.org/) lightweight IDE. It adds a command to automatically delete duplicate lines, leaving only the first line in a sequence of duplicates.

For example, consider a file containing this:

    foo
    bar
    bar
    bar
    baz
    baz
    foo

After running Geanyuniq on this document, the contents would be:

    foo
    bar
    baz
    foo


##Installation##
At the moment, Geanyuniq is not available in any pre-packaged form, so you need to build it yourself. Luckily, that isn't too hard. The following steps should do it:

1. Grab the code (`git clone git://github.com/unwind/geanyuniq.git`)
2. Using a terminal, enter the top-level `geanyuniq/` directory
3. Type `make`
4. Type `sudo ln -s $(pwd)/src/geanyuniq.so $(dirname $(dirname $(which geany)))/lib/geany`

The final step is the most complicated; the purpose is to create a symbolic link from Geany's system-wide plugin directory to the library you just built. This assumes that you will want to keep the plugin in the source directory; if you rather would install it permanently you can move it to the indicated location instead, and then delete the source code.

Keeping a symbolic link will of course make it easier to update the plugin if there are any changes (or if you do changes yourself).


#Running Geanyuniq#
By default, Geanyuniq assigns itself to the keyboard shortcut Shift+Control+D. This is quite natural, since Control+D in turn is bound to the "Duplicate Line or Selection" command by default.


##Finding Geanyuniq in the Menus##
Geanyuniq tries to insert its menu item in Geany's "Edit" menu, so that it appears right after the "Duplicate Line or Selection" command. This is a bit non-standard, adventurous programming, and thus might break. The insertion is based on the phrasing of the text in Geany's built-in menus, and will not cope with running Geany in non-English.

To fix this, edit the `MENU_EDIT_LABEL` and `MENU_DUPLICATE_LABEL` strings at the top of the source code, replacing the default English text with your desired locale's.

The menu installation code tries to fall back to just appending to the Tools menu if it fails to find the "Edit" menu.


##Configuring Geanyuniq##
Currently, Geanyuniq has no configuration options.


#Feedback#
Please contact the author, Emil Brink (by e-mailing &lt;emil@obsession.se&gt;) regarding any bugs, comments, or thoughts about Geanyuniq. Enjoy.
