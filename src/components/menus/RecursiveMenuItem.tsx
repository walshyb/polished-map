import { MenuItem, Menu, Icon, Label } from 'semantic-ui-react'
import type { FileNode } from '../../store/fileSlice'; 
import { useState } from 'react';

export default function RecursiveMenuItem(
  { fileNode }: { fileNode: FileNode }
) {
  const [open, setOpen] = useState(false);

  const handleToggle = () => {
    setOpen(!open);
  };


  // Render just a menu item if no children
  if (!fileNode.children || fileNode.children.length === 0) {
    return (
      <MenuItem
        name={fileNode.name}
        active={false}
        inverted
        style={{ display: 'flex', alignItems: 'baseline', width: '100%' }}
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
      onClick={() => { handleToggle() }}
    >
      <span
        style={{ display: 'flex', justifyContent: 'space-between', width: '100%' }}
      >
        {fileNode.name} <Icon name={open ? 'caret down' : 'caret right'} />
      </span>

      { open && fileNode.children && (
        <Menu
          vertical
          inverted
          onClick={() => { handleToggle() }}
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
