<?php

/**
 * This class has been auto-generated by the Doctrine ORM Framework
 */
class Comment extends BaseComment
{
	public function getObject() {
		if ($this->getApplicationId())
			return $this->getApplication();
		if ($this->getMaduleId())
			return $this->getMadule();
		if ($this->getThemeId())
			return $this->getTheme();
		return null;
	}

	public function getXML($rss = false) {
		if ($rss) {
			$output = 	'<item>' .
							'<title>Comment for '.htmlspecialchars($this->getObject()->getName()).' by '.htmlspecialchars($this->getUser()->getName()).'</title>' .
							'<link>'.htmlspecialchars(Tools::get('url').'/comment/show/'.$this->getId()).'</link>' .
							'<description>'.htmlspecialchars($this->getComment()).'</description>' .
							'<pubDate>'.date('r', strtotime($this->getUpdatedAt())).'</pubDate>' .
							'<guid>'.htmlspecialchars(Tools::get('url').'/comment/show/'.$this->getId()).'</guid>' .
						'</item>';
		} else {
			$output = '<comment>';
			$output .= '<id>'.$this->getId().'</id>';
			$output .= '<message>'.htmlspecialchars($this->getComment()).'</message>';
			if ($this->getObject()->getObjectType()==Object::THEME)
				$output .= '<theme_id>'.$this->getObjectId().'</theme_id>';
			else
				$output .= '<module_id>'.$this->getObjectId().'</module_id>';
			$output .= '<user_id>'.$this->getUserId().'</user_id>';
			$output .= '<created_at>'.$this->getCreatedAt().'</created_at>';
			$output .= '<updated_at>'.$this->getUpdatedAt().'</updated_at>';
			$output .= '</comment>';
		}
		return $output;
	}
	
	public function save(Doctrine_Connection $conn = null) {
    	if (!$this->exists()) {
			$this->setCreatedAt(date('Y-m-d H:i:s'));
    	}
		$this->setUpdatedAt(date('Y-m-d H:i:s'));
		parent::save($conn);
	}
	
	public static function update($values, $user) {
		$comment = new Comment();
		$comment->setComment(substr($values['comment'], 0, 5000));
		if ($values['application_id'])
			$comment->setApplicationId($values['application_id']);
		if ($values['module_id'])
			$comment->setMaduleId($values['module_id']);
		if ($values['theme_id'])
			$comment->setThemeId($values['theme_id']);
		$comment->setUserId($user->getId());
		$comment->save();
		return $comment;
	}
}