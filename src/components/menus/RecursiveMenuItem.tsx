import { MenuHeader, MenuItem, Menu, Icon, Label } from 'semantic-ui-react'
import type { FileNode } from '../../store/fileSlice'; 
import { useState } from 'react';
import { openFileByName } from '../../store/fileSlice';
import { useAppDispatch } from '../../hooks';

export default function RecursiveMenuItem(
  { fileNode }: { fileNode: FileNode }
) {
  const [open, setOpen] = useState(false);
  const dispatch = useAppDispatch();

  const handleToggle = () => {
    setOpen(!open);
  };

  const handleOpenFile = (path: string, filename: string) => {
    dispatch(
      openFileByName({path: fileNode.path, name: fileNode.name})
    );
  };

  // Render just a menu item if no children
  if (!fileNode.children || fileNode.children.length === 0) {
    return (
      <MenuItem
        name={fileNode.name}
        active={false}
        inverted
        style={{ display: 'flex', alignItems: 'baseline', width: '100%' }}
        onClick={ () => handleOpenFile(fileNode.path || '', fileNode.name) }
      >
        {fileNode.name}
      </MenuItem>
    );
  }

  // Render a menu item with children
  return (
    <MenuItem
      name={fileNode.name}
      active={false}
      inverted
    >
      <MenuHeader
        style={{ display: 'flex', justifyContent: 'space-between', width: '100%' }}
        onClick={() => { handleToggle() }}
      >
        {fileNode.name} <Icon name={open ? 'caret down' : 'caret right'} />
      </MenuHeader>

      { open && fileNode.children && (
        <Menu
          vertical
          inverted
        >
          { fileNode.children.map((child, index) => {
            return (
              <RecursiveMenuItem
                key={index}
                fileNode={child}
              />
            );
          })}
        </Menu>
      )}
    </MenuItem>
  );
}
